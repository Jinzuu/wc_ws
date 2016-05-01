
/*****************************************
 *  INCLUDES
 *****************************************/
#include "stdint.h"
#include "system_stm32f4xx.h"
#include "main.h"
// UB Libs
#include "ub_lib/stm32_ub_systick.h"
#include "ub_lib/stm32_ub_led.h"
#include "ub_lib/stm32_ub_dig_out.h"
#include "ub_lib/stm32_ub_dig_in.h"
#include "ub_lib/stm32_ub_adc1_single.h"
#include "ub_lib/stm32_ub_rtc.h"
#include "ub_lib/stm32_ub_tim2.h"
#include "ub_lib/stm32_ub_ws2812.h"
#include "ub_lib/stm32_ub_uart.h"
#include "sonstige_lib/stm32_ub_tim5.h"
// Own Stuff
#include "DCF77.h"
#include "wc_frontend.h"
#include "wc_IrRemoteProcessing.h"
#include "wc_TimeToMatrix.h"
#include "wc_LightDependentBrightness.h"
#include "CommonMath.h"
#include "IRMP/irmp.h"
#include "esp8266.h"

/*****************************************
 *  GLOBALS
 *****************************************/

volatile BitAction gDcfInputState = Bit_RESET;
volatile BitAction gDcfRxWasSuccesful = Bit_RESET;
// If true, time is deemed invalid and word matrix is disabled
volatile BitAction gDcfRxInProgress = Bit_SET;
// Stores the last time a DCF Rx was successful. If too old, time is deemed invalid
RTC_t gLastSuccessfulDcfRxTime;

// As Refresh of word matrix depends on an IRQ, it is not possible to call it in an IRQ. Use this instead
volatile BitAction gWcIsToBeRefreshed = Bit_RESET;


// IR globals
IRMP_DATA irData;

/*****************************************
 *  Function declarations
 *****************************************/
// Returns 1 (BIT_SET) if time is not older than the DCF_ACCEPTED_AGE_IN_HOURS
BitAction DcfTimeWasSetRecently( void );

/*****************************************
 *  MAIN
 *****************************************/
int main(void)
{
	SystemInit();
	UB_Systick_Init();

	// Init of UB libs
	UB_TIMER2_Init_FRQ( 100 );
	UB_TIMER5_Init_FRQ( 10000 );

	UB_Led_Init();

	UB_DigIn_Init();
	UB_DigOut_Init();

	UB_ADC1_SINGLE_Init();

	UB_RTC_Init();
	UB_RTC_SetWakeUpInterrupt(RTC_WAKEUP_5s);

	// Note: code needs to be reconfigured for Nucleo Board (Frequency of 96 MHz should also be checked)
	UB_WS2812_Init();
	WC_DisableAllElements();
	WC_SetColor( WS2812_HSV_COL_WHITE );
	WC_SetBrightness( 10 );
	WC_SetElement(WC_ELEMENT_ES, 1);
	WC_Refresh();

	UB_Uart_Init();
	esp8266_init();

	UB_Systick_Pause_ms(1000);

	// Indicate successful booting
	UB_Led_On( LED_GREEN );
	UB_Systick_Pause_s(1);
	UB_Led_Off( LED_GREEN );

	// Start timers and therefore cyclic actions in the call backs below
	UB_TIMER2_Start();
	UB_TIMER5_Start();


	UB_DigOut_Lo(DOUT_PB7);	// Set ground for LDR
	UB_DigOut_Lo(DOUT_PB9);	// Set PC9 low to start DCF module

	while(1) {
		// Handle word matrix refreshes
		if ( gWcIsToBeRefreshed == Bit_SET ){
			WC_Refresh();
			gWcIsToBeRefreshed = Bit_RESET;
		}

		// Check if update of time is necessary
#ifndef DISABLE_DCF
		if ( DcfTimeWasSetRecently() == Bit_RESET )
			gDcfRxInProgress = Bit_SET;
#endif

		// Handle IR remote
		if ( irmp_get_data( &irData ) )
			ProcessIrDataPacket( irData );

		// Read Ambient brightness and set LED brightness
		if ( gDcfRxInProgress == Bit_RESET ){
			ambientBrightnessCurrent = SlidingAverageOnLastValues( UB_ADC1_SINGLE_Read( ADC_PA1 ) );
			int brightnessToSet = 100.0 * GetBrightnessFactor( ambientBrightnessPoints, ambientBrightnessLedDimmingFactors, ambientBrightnessCurrent );
			if ( brightnessToSet < LED_BRIGHTNESS_OFF_THRESHOLD )
				WC_SetColor( WS2812_HSV_COL_OFF );
			else
				WC_SetBrightness( brightnessToSet );
			gWcIsToBeRefreshed = Bit_SET;
		}

		// Handle ESP8266 receive
		esp8266_handle_receive();
		if( esp8266_request_time_from_google() == 1 ) {
			UB_RTC = Esp8266_curTime;
			UB_RTC_SetClock( RTC_DEC );
			SetWordMatrix( UB_RTC_GetClock(RTC_DEC) );
			gWcIsToBeRefreshed = Bit_SET;
		}
	}

}


/*****************************************
 *  Timer 2 Callback @100Hz
 *  - Used for: DCF77 sampling
 *****************************************/
void UB_TIMER2_ISR_CallBack( void )
{
#ifdef DISABLE_DCF
	return;
#endif

	gDcfInputState = UB_DigIn_Read( DIN_PB8 );
	if ( gDcfInputState == Bit_SET )
		UB_Led_On( LED_GREEN );
	else
		UB_Led_Off( LED_GREEN );

#ifdef ENABLE_TESTMODE	//Testing: Just set a time
	gLastSuccessfulDcfRxTime.jahr = 0;
	gLastSuccessfulDcfRxTime.monat = 1;
	gLastSuccessfulDcfRxTime.tag = 1;
	gLastSuccessfulDcfRxTime.wotag = 1;
	gLastSuccessfulDcfRxTime.sek = 0;
	gLastSuccessfulDcfRxTime.min = 0;
	gLastSuccessfulDcfRxTime.std = 0;

	WC_SetElement(WC_ELEMENT_ES, 1);
	SetWordMatrix( UB_RTC_GetClock(RTC_DEC) );
	gDcfRxInProgress = Bit_RESET;
	gDcfRxWasSuccesful = Bit_SET;
	gCurrentMatrixColor = WS2812_HSV_COL_WHITE;

#else
	dcf77_SignalState_t dcf77state = Dcf77_ProcessSignal( gDcfInputState );
	if ( dcf77state == dcf77_TimeRxSuccess )
	{
		gLastSuccessfulDcfRxTime = Dcf77_GetTime();
		UB_RTC = gLastSuccessfulDcfRxTime;
		UB_RTC_SetClock( RTC_DEC );
		gDcfRxWasSuccesful = Bit_SET;

		// Set word matrix directly after first DCF RX
		if ( gDcfRxInProgress == Bit_SET ){
			WC_SetElement(WC_ELEMENT_ES, 1);
			SetWordMatrix( UB_RTC_GetClock(RTC_DEC) );
			gDcfRxInProgress = Bit_RESET;
			gCurrentMatrixColor = WS2812_HSV_COL_WHITE;
		}
	}
	// Indicate status of DCF reception by FUNK in red/green while DCF RX in progress
	if ( gDcfRxWasSuccesful == Bit_RESET ){
		WC_DisableAllElements();
		WC_SetElement( WC_ELEMENT_FUNK, 1 );
		if ( dcf77state == dcf77_RxStateUnkown )
			WC_SetColor( WS2812_HSV_COL_RED );
		else if ( dcf77state == dcf77_RxStateGood )
			WC_SetColor( WS2812_HSV_COL_GREEN );

		WC_SetBrightness( 50 );
		WC_SetElement( WC_ELEMENT_FUNK, 1 );
	}
	else{
		// Disable FUNK and set normal color
		WC_SetElement( WC_ELEMENT_FUNK, 0 );
		WC_SetColor( gCurrentMatrixColor );
	}
#endif

	gWcIsToBeRefreshed = Bit_SET;
}

/*****************************************
 *  Timer 5 Callback @10kHz
 *  - Used for: IR Remote sampling
 *****************************************/
void UB_TIMER5_ISR_CallBack( void )
{
	// routine for IR remote signal processing
	irmp_ISR( UB_DigIn_Read( DIN_PA15 ) );
}

// Other non used timer callbacks
void UB_TIMER3_ISR_CallBack( void )
{

}

void UB_TIMER4_ISR_CallBack( void )
{

}

/*****************************************
 *  RTC Wakeup Callback
 *  - Used for: Setting the word matrix
 *****************************************/
int WC_OneMinute_ISR_Count = 0;
void WC_OneMinute_ISR()
{
	if( WC_OneMinute_ISR_Count >= 1 ) {
		WC_OneMinute_ISR_Count = 0;

		// Get time to UB_RTC global and use it to set the matrix
		if ( gDcfRxWasSuccesful == Bit_SET ) {
			SetWordMatrix( UB_RTC_GetClock(RTC_DEC) );
			gWcIsToBeRefreshed = Bit_SET;
		} else if( Esp8266_curTime.status == RTC_TIME_OK ) {
			SetWordMatrix( UB_RTC_GetClock(RTC_DEC) );
			gWcIsToBeRefreshed = Bit_SET;
		}
	} else {
		WC_OneMinute_ISR_Count++;
	}
}

/*****************************************
 *  Function definitions
 *****************************************/

// Returns 1 (BIT_SET) if time is not older than the DCF_ACCEPTED_AGE_IN_HOURS
BitAction DcfTimeWasSetRecently( void ){
	RTC_t currentTime = UB_RTC_GetClock(RTC_DEC);
	int timeDifferenceInHours = currentTime.std - gLastSuccessfulDcfRxTime.std;
	int timeDifferenceInDays = currentTime.tag - gLastSuccessfulDcfRxTime.tag;
	if ( timeDifferenceInDays < 0 ){
		switch ( gLastSuccessfulDcfRxTime.monat ) {
			case 2:	// Februar
				timeDifferenceInDays += 28;
				break;
			case 4:	// April
				timeDifferenceInDays += 30;
				break;
			case 6:	// Juni
				timeDifferenceInDays += 30;
				break;
			case 9:	// September
				timeDifferenceInDays += 30;
				break;
			case 11:// November
				timeDifferenceInDays += 30;
				break;
			default:// Rest
				timeDifferenceInDays += 31;
				break;
		}
	}

	timeDifferenceInHours += timeDifferenceInDays * 24;
	if ( timeDifferenceInHours < DCF_ACCEPTED_AGE_IN_HOURS )
		return Bit_SET;
	return Bit_RESET;
}
