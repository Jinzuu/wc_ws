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
#include "ub_lib/stm32_ub_rtc.h"
// Own Stuff
#include "ws2811.h"
#include "DCF77.h"

/*****************************************
 *  GLOBALS
 *****************************************/

volatile uint8_t gNewDcfSampleAvailable = 0;
volatile BitAction gDcfInputState = Bit_RESET;

/*****************************************
 *  MAIN
 *****************************************/
int main(void)
{
	SystemInit();

	// Init of UB libs
	UB_Systick_Init();
	UB_TIMER2_Init_FRQ( 100 );

	UB_Led_Init();
	UB_DigIn_Init();
	UB_DigOut_Init();

	ws2811_init();

	UB_Led_On( LED_GREEN );
	UB_Systick_Pause_ms( 100 );
	// Start timers and therefore cyclic actions in the call backs below
	UB_TIMER2_Start();

	while(1) {
		UB_DigOut_Lo(DOUT_PB7);
		setLED(0, 255, 128, 128);
		updateLED();
		while( DMA_BUSY == TRUE ) {
			UB_Systick_Pause_ms( 1 );
		}
	}


}


/*****************************************
 *  Timer 2 Callback @100Hz
 *  - Used for: DCF77 sampling
 *****************************************/
void UB_TIMER2_ISR_CallBack( void )
{
	UB_Led_On( LED_BLUE );
	gDcfInputState = UB_DigIn_Read( DIN_PD3 );
	if ( gDcfInputState == Bit_SET )
		UB_Led_On( LED_ORANGE );
	else
		UB_Led_Off( LED_ORANGE );
	gNewDcfSampleAvailable = 1;
	UB_Led_Off( LED_BLUE );
}

// Other non used timer callbacks
void UB_TIMER3_ISR_CallBack( void )
{

}

void UB_TIMER4_ISR_CallBack( void )
{

}

void UB_TIMER5_ISR_CallBack( void )
{

}
