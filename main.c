/*********************************************
 * Wordclock realization with the NUCLEO-411RE
 * Note: Due to lack of support for the 411, CoIDE has been configured to the 401
 *********************************************/

/*****************************************
 *  INCLUDES
 *****************************************/
#include "stdint.h"
#include "system_stm32f4xx.h"
// UB Libs
#include "ub_lib/stm32_ub_systick.h"
#include "ub_lib/stm32_ub_led.h"
#include "ub_lib/stm32_ub_dig_out.h"
#include "ub_lib/stm32_ub_dig_in.h"
#include "ub_lib/stm32_ub_adc1_single.h"
#include "ub_lib/stm32_ub_rtc.h"
#include "ub_lib/stm32_ub_tim2.h"
#include "ub_lib/stm32_ub_ws2812.h"
#include "sonstige_lib/stm32_ub_tim5.h"
// Own Stuff
#include "DCF77.h"
#include "wc_frontend.h"
#include "wc_IrRemoteProcessing.h"
#include "wc_TimeToMatrix.h"
#include "wc_LightDependentBrightness.h"
#include "CommonMath.h"
#include "IRMP/irmp.h"


/*****************************************
 *  USED PINS on NUCLEO F411 RE Board
 *****************************************/
// Note: Where pin position description is given, directions left/right are
// 	when holding the board so that text on it is readable (USB connector facing away)

// ##### DCF77 #####
// PC8 - DataIn for DCF77 module (Right morpho connector, top right pin)
// PC9 - PowerOn Pin of DCF77 module (Right morpho connector, top left pin)
// 		Additional pins for DCF:
//			+3.3V  (Right morpho connector, 4th pin from top, left row)
//			GND  (Right morpho connector, 5th pin from top, left row)

// ##### LDR #####
// PA1 - AnalogIn for LDR (Left Arduino analog connector, named A1)
// 		Note: To be connected by ext wire to PC13
// PB7 - Gpio Pulled Low: Ground for LDR (left morpho connector, left row, 9th pin from bottom)
// PC13 - PullUp for LDR (left morpho connector, left row, 8th pin from bottom)
//		Note: Used for the Onboard Pullup resistor for LDR (originally user button pullup)

// ##### IR REMOTE #####
// PA15 - IR Remote digital in (left morpho connector, left row, 10th pin from bottom)
// 		Additional pins for IR remote:
//			GND  (left morpho connector, left row, 8th pin from bottom)
//			+3.3V  (left morpho connector, left row, 3rd pin from top)


// ##### WS2812 LED STRIPE #####
// PB4 - PWM Out for Led Control (Ext. Pullup to 5V needed!) (marked as PWM/D5 on board)
// 		Note: Suggestion for 5V Pullup: Take a 4.7k Wired Resistor and stick it into top left Arduino
//				connector VIN and into bottom right arduino connector PWM/D5 (should be just ok in length)

/*****************************************
 *  DEFINES
 *****************************************/
#define LED_BRIGHTNESS_OFF_THRESHOLD	1	// in percent 1...100

/*****************************************
 *  GLOBALS
 *****************************************/

volatile BitAction gDcfInputState = Bit_RESET;
volatile BitAction gDcfRxWasSuccesful = Bit_RESET;
volatile BitAction gWordMatrixWasSetForTheFirstTime = Bit_RESET;

// IR globals
IRMP_DATA irData;


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
	WC_SetColor(WS2812_HSV_COL_WHITE);
	WC_SetElement(WC_ELEMENT_ES, 1);
	WC_Refresh();

	// Indicate successful booting
	UB_Led_On( LED_GREEN );
	UB_Systick_Pause_s(1);
	UB_Led_Off( LED_GREEN );

	// Start timers and therefore cyclic actions in the call backs below
	UB_TIMER2_Start();
	UB_TIMER5_Start();


	UB_DigOut_Lo(DOUT_PB7);	// Set ground for LDR
	UB_DigOut_Lo(DOUT_PC9);	// Set PC9 low to start DCF module


	while(1) {
		// Handle IR remote
		if ( irmp_get_data( &irData ) )
			ProcessIrDataPacket( irData );

		// Read Ambient brightness and set LED brightness
		ambientBrightnessCurrent = SlidingAverageOnLastValues( UB_ADC1_SINGLE_Read( ADC_PA1 ) );
		int brightnessToSet = 100.0 * GetBrightnessFactor( ambientBrightnessPoints, ambientBrightnessLedDimmingFactors, ambientBrightnessCurrent );
		if ( brightnessToSet < LED_BRIGHTNESS_OFF_THRESHOLD )
			WC_DisableAll();
		else
			WC_SetBrightness( brightnessToSet );
	}

}


/*****************************************
 *  Timer 2 Callback @100Hz
 *  - Used for: DCF77 sampling
 *****************************************/
void UB_TIMER2_ISR_CallBack( void )
{
	gDcfInputState = UB_DigIn_Read( DIN_PC8 );
	if ( gDcfInputState == Bit_SET )
		UB_Led_On( LED_GREEN );
	else
		UB_Led_Off( LED_GREEN );

	dcf77_SignalState_t dcf77state = Dcf77_ProcessSignal( gDcfInputState );
	if ( dcf77state == dcf77_TimeRxSuccess )
	{
		UB_RTC = Dcf77_GetTime();
		UB_RTC_SetClock( RTC_DEC );
		gDcfRxWasSuccesful = Bit_SET;

		// Set word matrix directly after first DCF RX
		if ( gWordMatrixWasSetForTheFirstTime == Bit_RESET ){
			SetWordMatrix( UB_RTC_GetClock(RTC_DEC) );
			gWordMatrixWasSetForTheFirstTime = Bit_SET;
		}
	}
	// Indicate status of DCF reception by top left LED in red/green while DCF RX in progress
	if ( gDcfRxWasSuccesful == Bit_RESET ){
		WC_SetElement( WC_ELEMENT_MIN_4, 1 );
		if ( dcf77state == dcf77_RxStateUnkown )
			WC_SetColor( WS2812_HSV_COL_RED );
		else if ( dcf77state == dcf77_RxStateGood )
			WC_SetColor( WS2812_HSV_COL_GREEN );
		WC_SetElement( WC_ELEMENT_MIN_4, 1 );
	}
	else{
		// Disable top left led and set normal color
		WC_SetElement( WC_ELEMENT_MIN_4, 0 );
		WC_SetColor( gCurrentMatrixColor );
	}
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
		if ( gDcfRxWasSuccesful == Bit_SET )
			SetWordMatrix( UB_RTC_GetClock(RTC_DEC) );
	} else {
		WC_OneMinute_ISR_Count++;
	}
}
