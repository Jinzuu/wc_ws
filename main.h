/*********************************************
 * Wordclock realization with the NUCLEO-411RE
 * Note: Due to lack of support for the 411, CoIDE has been configured to the 401
 *********************************************/

/*****************************************
 *  INCLUDES
 *****************************************/

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
// PB4 - PWM Out for Led Control (Ext. Pullup to 5V needed and series resistor) (marked as PWM/D5 on board)
// 		Note: Suggestion for 5V Pullup: Connect to E5V by 4.7k resistor on bottom side
// 		Note: Adding a series resistor is recommended to reduce EMI (better for DCF) and not to overpower first LED

/*****************************************
 *  DEFINES
 *****************************************/
#define LED_BRIGHTNESS_OFF_THRESHOLD	1	// in percent 1...100
#define DCF_ACCEPTED_AGE_IN_HOURS		96

// If testmode is enabled, no DCF time is caught but lights are turned directly
// Also, pushing the smooth button cycles through time (adding 1h 3min per click)
#define ENABLE_TESTMODE

/*****************************************
 *  GLOBALS
 *****************************************/


/*****************************************
 *  Function declarations
 *****************************************/

