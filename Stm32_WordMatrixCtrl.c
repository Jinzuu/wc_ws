// /***********************************************************************
// * Stm32_Wordclock.h
// *
// *  Created on: 11.01.2014
// *      Author: Jan Berg
// *      Device: Stm32F4007 Discovery
// *      Purpose: LowCost Wordclock :)
// *
// ***********************************************************************/
//
//
///**********************************************************
// *                     Defines
// **********************************************************/
//#define MINIMUM(A, B)  ( (A) < (B) ? (A) : (B) )
//#define MAXIMUM(A, B)  ( (A) > (B) ? (A) : (B) )
//
//// General defines
//#define USE_ALTERNATIVE_IR_REMOTE	// Use DX remote commands to control clock
//#define KEYPRESSES_COUNT_FOR_SPECIALACTION	15	// By holding a key for a longer time period, special adjustments can be made
//#define SPECIAL_MODE_DURATION_AFTER_KEYPRESS	10	//in seconds before switchback to normal mode
//#define N_POWER_ON_PRESS_TO_IGNORE_DCFSYNC_AND_ENABLE_TESTMODE	5
//
//// Due to exponential dimming, the current brightness the following factor is multiplied by this
//// factor for higher brightness and divided by it for lower brightness.
//#define DIM_DURATION	3	// Wait time in seconds for up/down dimming during word matrix change
//#define HUE_INCREMENT	0.01	// Defines how much the hue is incremented in each step in color change mode
//
//// Due to exponential dimming, the current brightness the following factor is multiplied by this
//// factor for higher brightness and divided by it for lower brightness.
//#define BRIGHTNESS_DIM_FACTOR	1.25
//#define MIN_BRIGHTNESS_MULTI_DIFF	0.0001	// Minimal brightness multiplication difference allowed between indices in the multiplication arrays
//#define N_AMBIENT_BRIGHTNESS_CONVERSION_PTS	8 // Number of points with conversion factors from ambient to led brightness
//
//#define PIN_IR_DATA		4
//
//#define PIN_DCF_DATA	5
//#define PIN_DCF_PON		6
//
//#define PIN_LED_OK		19
//
//// Debug defines
//// if defined, the clock is in "normal" mode and activates itself once it has received dcf time
//// if not defined, the timing and displaying of the time starts directly (for testing)
//#define USE_DCF77
////#define DEBUG_SPACE_SAVING	// comments out: color changing mode, ir commands etc.
//
//
//// PWM PIN DEFINES
//#define PWM_CHANNELS	0x3F	// All necessary with ambilight
////#define PWM_CHANNELS	0x07	// Only PWM for the words
//#define PWM_CLOCK_RED_CH	1
//#define PWM_CLOCK_RED_PIN	9
//#define PWM_CLOCK_GREEN_CH	2
//#define PWM_CLOCK_GREEN_PIN	8
//#define PWM_CLOCK_BLUE_CH	3
//#define PWM_CLOCK_BLUE_PIN	7
//#define PWM_AMBI_RED_CH		4
//#define PWM_AMBI_RED_PIN	1
//#define PWM_AMBI_GREEN_CH	5
//#define PWM_AMBI_GREEN_PIN	0
//#define PWM_AMBI_BLUE_CH	6
//#define PWM_AMBI_BLUE_PIN	21
//
//// Single LED PIN Defines
//#define LED_MIN_DOT1	18
//#define LED_MIN_DOT2	17
//#define LED_MIN_DOT3	16
//#define LED_MIN_DOT4	15
//
//#define LED_ES_IST		13
//#define LED_MIN_FUENF	26
//#define LED_MIN_ZEHN	25
//#define LED_MIN_ZWANZIG	12
//
//// Shift registers
//#define SHIFT_HOURS_CLK	22
//#define SHIFT_HOURS_IN	23
//#define SHIFT_MINS_CLK	10
//#define SHIFT_MINS_IN	11
//
//// ###### Shift Register Bit Positions ######
//// Minute shift register
//#define BIT_POSITION_VIERTEL	0x01
//#define BIT_POSITION_NACH	0x02
//#define BIT_POSITION_VOR	0x04
//#define BIT_POSITION_HALB	0x08
//#define BIT_POSITION_ZWOELF	0x10
//
//// hour lower shift register
//#define BIT_POSITION_ZW	0x01
//#define BIT_POSITION_EI	0x02
//#define BIT_POSITION_N	0x04
//#define BIT_POSITION_S	0x08
//#define BIT_POSITION_IEBEN	0x10
//#define BIT_POSITION_DREI	0x20
//#define BIT_POSITION_FUENF	0x40
//#define BIT_POSITION_ELF	0x80
//
//// hour higher shift register
//#define BIT_POSITION_NEUN	0x01
//#define BIT_POSITION_VIER	0x02
//#define BIT_POSITION_ACHT	0x04
//#define BIT_POSITION_ZEHN	0x08
//#define BIT_POSITION_SECHS	0x10
//#define BIT_POSITION_UHR	0x20
//
///**********************************************************
// *                     Declarations
// **********************************************************/
//
// typedef enum{
// min_Fuenf,
// min_Zehn,
// min_Viertel,
// min_Zwanzig,
// min_Halb,
// min_Vor,
// min_Nach,
// std_Ein,
// std_S,
// std_Zwei,
// std_Drei,
// std_Vier,
// std_Fuenf,
// std_Sechs,
// std_Sieben,
// std_Acht,
// std_N, //for zeh_N_ and _N_eun
// std_Eun,
// std_Zehn,	// last n is at std_N
// std_Elf,
// std_Zwoelf,
// Funk
// };
//
//
//
//void SetWordMatrix( int hourToShow );
//void SetMinuteDotLeds();
//
//bool TimeWasSetRecently();
//bool DcfReceiveModeIsActive();
//
//
//
///**********************************************************
// *                      Static variables
// **********************************************************/
//
//// IR statics
//volatile static IRMP_DATA currentIrData;
//volatile static int	firstSeenIrRemoteAddress = -1;	// ID not set yet
//volatile static unsigned irCommandRepetitionCount = 0;
//volatile static unsigned lastIRCommandReceived = 11;
//volatile static int lastIrDataRx100HzTime = -1;
//
//// Time statics
//volatile static int lastDcfSyncDay = -10;	// force initial sync;
//volatile static int lastDcfSyncHour = 0;
//
//// Time based automatic on/off for the clock
//static bool autoOnOffActive = false;
//static int autoOnOffWeekdayOffHour = 10;
//static int autoOnOffWeekdayOnHour = 17;
//static bool autoOnOffWasSwitchedOff = false;
//static bool autoOnOffWasSwitchedOn = false;
//static bool autoOnOffSettingChanged = true;
//static int autoOnOffIndicationCounter = 0;
//
//// Color related stuff
//volatile static hsvColor colorClock;
//volatile static hsvColor colorAmbi;
//
//volatile static bool clockIsOff = false;
//volatile static bool ambilightIsOff = false;
//
//// Last seen averaged ambient brightness
//volatile static float ambientBrightness = 0.1;
//
//// conversion from seen ambient brightness to brightness multiplier (nonlinear stuff...)
//static float ambientBrightnessPoints[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS] = {0.0, 0.01, 0.05, 0.1, 	0.2, 	0.33, 	0.5, 1.0};
//static float ambientMultiplierPointsClock[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS] = {0.1, 0.15, 0.25, 0.35, 0.45, 	0.6, 	0.8, 1.0};
//static float ambientMultiplierPointsAmbi[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS] = {0.1, 0.15, 0.25, 0.35, 0.45, 	0.6, 	0.8, 1.0};
//
//volatile static int dimCounter = - 1;	// for dimming the wordmatrix during changes
//
//// For the shift register
//// Describes, which shift registers are to fill (0=none, 1=minute, 2=higherHour, 3=lowerHour)
//volatile static unsigned currentShiftRegToFill = 1; // Disable everything at startup once
//volatile static unsigned shiftBitNumber = 0;
//volatile static bool shiftClockIsHigh = 0;
//// Reflect the active state of the shift registers. Pinlayout is as follows:
//// MINUTE:		1->  HALB, 2-> NACH, 3-> VOR, 4-> , 5-> , 6-> , 7-> , 8->
//// HOUR_LOW:	1-> N (eiNs), 2-> ZW (ZWei), 3-> DREI, 4-> VIER, 5-> FUENF, 6-> SECHS, 7-> IEBEN (sIEBEN), 8-> ACHT
//// HOUR_HIGH:	1-> NEUN, 2-> ZEHN, 3-> ELF, 4-> ZWOELF, 5-> EI (EIns), 6-> S (einS/Sieben), 7-> ES IST, 8-> UHR
//volatile static char minuteShiftRegContent = 0x0;
//volatile static char hourShiftRegContentLow = 0x0;
//volatile static char hourShiftRegContentHigh = 0x0;
//
//// For special functions, where non-normal behaviour is necessary (e.g. blinking LEDs while adjusting on/off times)
//volatile static bool ledEsIstIsOn = true;
//volatile static unsigned CycleCounter100Hz = 0; // used for uncritical waiting in special modes (light modulation etc)
//volatile static float colorRotationHueIncrementClock = 0;	// Help adjusting color changing speed
//volatile static float colorRotationHueIncrementAmbi = 0;
//static bool colorRotationModeHasChanged = false;
//static int colorRotationLedIndicationCounter = 200;
//volatile static clockMode currentClockMode = normal;
//volatile static int specialModeEntrySecond = -1;	// -1 -> not set initially
//volatile static int nPowerOnPressesInDcfSyncMode = 0;
//
///**********************************************************
// *                      Interrupts
// **********************************************************/
//// TIMER0 ISR -> Fast 10 KHZ ISR FOR SHIFT REGS AND IR
//void __attribute__ ((interrupt("IRQ")))  TIMER0_ISR(void)
//{
//	// routine for IR remote
//	uint_fast8_t irPinIsHigh = IOPIN & ( 1 << PIN_IR_DATA );
//	irmp_ISR( irPinIsHigh );
//
//#ifndef DEBUG_SPACE_SAVING
//	// Set shift register contents
//	switch( currentShiftRegToFill )
//	{
//		case 1:
//			if( MoveBitpatternIntoShiftReg( SHIFT_MINS_CLK, SHIFT_MINS_IN, minuteShiftRegContent ) )
//				++currentShiftRegToFill;
//			break;
//		case 2:
//			if( MoveBitpatternIntoShiftReg( SHIFT_HOURS_CLK, SHIFT_HOURS_IN, hourShiftRegContentHigh ) )
//				++currentShiftRegToFill;
//			break;
//		case 3:
//			if( MoveBitpatternIntoShiftReg( SHIFT_HOURS_CLK, SHIFT_HOURS_IN, hourShiftRegContentLow ) )
//				currentShiftRegToFill = 0;
//			break;
//	}
//#endif	//DEBUG_SPACE_SAVING
//
//	// End of interrupt: Clear IRQ Bit and reset VIC by writing zero into its address register
//	T0IR = 0xFF;	//(all possible IRQs of timer1 are reset)
//	VICVectAddr = 0;
//}
//
//// TIMER0 ISR -> 100 HZ ISR FOR DCF etc
//void __attribute__ ((interrupt("IRQ")))  TIMER1_ISR(void)
//{
//	CycleCounter100Hz = ++CycleCounter100Hz % 100;
//
//#ifdef USE_DCF77
//	// ### DCF77 and RTC Code
//	if ( MIN % 15 == 14 )
//		IOSET |= ( 1 << PIN_DCF_PON );	// Set DCF77 PON pin low -> deactivate DCF module (to avoid problems with a hung dcf module)
//	else
//		IOCLR |= ( 1 << PIN_DCF_PON );	// Pull DCF77 PON low -> activate DCF module
//
//	// Read DCF signal
//	bool dcfPinIsHigh = IOPIN & ( 1 << PIN_DCF_DATA );
//	if ( !dcfPinIsHigh )
//		IOCLR |= ( 1 << PIN_LED_OK );
//	else
//		IOSET |= ( 1 << PIN_LED_OK );
//	if ( Dcf77_ProcessSignal( dcfPinIsHigh ) )
//	{
//		rtcTime time = Dcf77_GetTime();
//		time.year += 2000;
//		lastDcfSyncDay = time.dayOfMonth;
//		lastDcfSyncHour = time.hour;
//		SetRtcTime( time );
//		nPowerOnPressesInDcfSyncMode = 0;
//		if ( ledEsIstIsOn )	// Reenable ES_IST (was disabled during DCF sync)
//			IOSET |= ( 1 << LED_ES_IST );
//	}
//#endif	//DEBUG_SPACE_SAVING
//
//	// Reset old MIN LED3 (bottom left) state after IR packet reception
//	if ( lastIrDataRx100HzTime >= 0 && ( ( lastIrDataRx100HzTime + 10 ) % 100 ) == CycleCounter100Hz  )
//	{
//		if( MIN % 5 > 2 )
//			IOSET |= ( 1 << LED_MIN_DOT3 );
//		else
//			IOCLR |= ( 1 << LED_MIN_DOT3 );
//		lastIrDataRx100HzTime = -1;
//	}
//
//	// End of interrupt: Clear IRQ Bit and reset VIC by writing zero into its address register
//	T1IR = 0xFF;	//(all possible IRQs of timer1 are reset)
//	VICVectAddr = 0;
//}
//
//void __attribute__ ((interrupt("IRQ")))  RTC_INCREMENT_ISR(void)
//{
//#ifndef DEBUG_SPACE_SAVING
//	// New minute -> Initiate setting of Word Matrix Leds
//	if ( TimeWasSetRecently() )
//	{
//		if ( MIN % 5 == 0 && dimCounter < 0 )
//			dimCounter = 0;	// start dimming and matrix change
//		else
//			SetMinuteDotLeds();
//	}
//#endif	//DEBUG_SPACE_SAVING
//
//	// Every minute, null number of presses in sync mode
//	nPowerOnPressesInDcfSyncMode = 0;
//
//	// End of interrupt: Reset VIC by writing zero into its address register
//	ILR |= 0x1;
//	VICVectAddr = 0;
//}
//
///**********************************************************
// *                Function implementations
// **********************************************************/
//
//// Note: If hourToShow is > 24 only hour leds get set and UHR is shown
//void SetWordMatrix( int hourToShow )
//{
//#ifndef DEBUG_SPACE_SAVING
//	minuteShiftRegContent = 0x0;
//	hourShiftRegContentLow = 0x0;
//	hourShiftRegContentHigh = 0x0;
//
//	// Normal setting of word matrix depending on minutes and hours
//	if ( hourToShow < 24 )
//	{
//		// #### Set directly wired LEDs
//		if ( ( MIN >= 5 && MIN < 10 ) // fuenf nach voll
//				|| ( MIN >= 25 && MIN < 30 )	// fuenf vor halb
//				|| ( MIN >= 35 && MIN < 40 )	// fuenf nach halb
//				|| ( MIN >= 55 ) )	// fuenf vor voll
//			IOSET |= ( 1 << LED_MIN_FUENF );
//		else
//			IOCLR |= ( 1 << LED_MIN_FUENF );
//
//		if ( ( MIN >= 10 && MIN < 15 )	// zehn nach voll
//				|| ( MIN >= 50 && MIN < 55 ) )	// zehn vor voll
//			IOSET |= ( 1 << LED_MIN_ZEHN );
//		else
//			IOCLR |= ( 1 << LED_MIN_ZEHN );
//
//		if ( ( MIN >= 20 && MIN < 25 )	// zwanzig nach voll
//				|| ( MIN >= 40 && MIN < 45 ) )	// zwanzig vor voll
//			IOSET |= ( 1 << LED_MIN_ZWANZIG );
//		else
//			IOCLR |= ( 1 << LED_MIN_ZWANZIG );
//
//		// set VIERTEL
//		if ( ( MIN >= 15 && MIN < 20 )
//				|| ( MIN >= 45 && MIN < 50 ) )
//			minuteShiftRegContent |= BIT_POSITION_VIERTEL;
//
//		// set HALB
//		if ( MIN >= 25 && MIN < 40 )
//			minuteShiftRegContent |= BIT_POSITION_HALB;
//
//		// Set VOR/NACH and UHR
//		if ( ( MIN >= 5 && MIN < 25 )	// Nach voll
//				|| ( MIN >= 35 && MIN < 40 ) )	// Nach halb
//			minuteShiftRegContent |= BIT_POSITION_NACH;
//		if ( ( MIN >= 25 && MIN < 30 )	// vor halb
//				|| MIN >= 40 )	// vor voll
//			minuteShiftRegContent |= BIT_POSITION_VOR;
//		if ( MIN < 5 )
//			hourShiftRegContentHigh |= BIT_POSITION_UHR;
//
//
//		if ( MIN >= 25 )
//			++hourToShow;
//	}
//	// Word matrix is used to show a time, disable everything except hour display
//	else
//	{
//		IOCLR |= ( 1 << LED_ES_IST ) | ( 1 << LED_MIN_FUENF ) | ( 1 << LED_MIN_ZEHN ) | ( 1 << LED_MIN_ZWANZIG );
//		if ( hourToShow >= 36 )	// Indicate times >13h through activating hour too
//			hourShiftRegContentHigh |= BIT_POSITION_UHR;
//	}
//
//
//	// Set hour LEDs
//	hourToShow = hourToShow % 12;
//
//	switch ( hourToShow )
//	{
//	case 1:
//		hourShiftRegContentLow |= BIT_POSITION_EI;
//		hourShiftRegContentLow |= BIT_POSITION_N;
//		if ( MIN > 4 )
//			hourShiftRegContentLow |= BIT_POSITION_S;
//		break;
//	case 2:
//		hourShiftRegContentLow |= BIT_POSITION_ZW;
//		hourShiftRegContentLow |= BIT_POSITION_EI;
//		break;
//	case 3:
//		hourShiftRegContentLow |= BIT_POSITION_DREI;
//		break;
//	case 4:
//		hourShiftRegContentHigh |= BIT_POSITION_VIER;
//		break;
//	case 5:
//		hourShiftRegContentLow |= BIT_POSITION_FUENF;
//		break;
//	case 6:
//		hourShiftRegContentHigh |= BIT_POSITION_SECHS;
//		break;
//	case 7:
//		hourShiftRegContentLow |= BIT_POSITION_S;
//		hourShiftRegContentLow |= BIT_POSITION_IEBEN;
//		break;
//	case 8:
//		hourShiftRegContentHigh |= BIT_POSITION_ACHT;
//		break;
//	case 9:
//		hourShiftRegContentHigh |= BIT_POSITION_NEUN;
//		break;
//	case 10:
//		hourShiftRegContentHigh |= BIT_POSITION_ZEHN;
//		break;
//	case 11:
//		hourShiftRegContentLow |= BIT_POSITION_ELF;
//		break;
//	case 0:
//		minuteShiftRegContent |= BIT_POSITION_ZWOELF;
//		break;
//	}
//
//	// Start filling the registers
//	currentShiftRegToFill = 1;
//#endif	// DEBUG_SPACE_SAVING
//}
//
//// Sets only the minute dot leds
//void SetMinuteDotLeds()
//{
//	IOCLR |= ( 1 << LED_MIN_DOT1 ) | ( 1 << LED_MIN_DOT2 ) | ( 1 << LED_MIN_DOT3 ) | ( 1 << LED_MIN_DOT4 );
//	if( MIN % 5 != 0 )
//		IOSET |= ( 1 << LED_MIN_DOT1 );
//	if( MIN % 5 > 1 )
//		IOSET |= ( 1 << LED_MIN_DOT2 );
//	if( MIN % 5 > 2 )
//		IOSET |= ( 1 << LED_MIN_DOT3 );
//	if( MIN % 5 > 3 )
//		IOSET |= ( 1 << LED_MIN_DOT4 );
//}
//
//// Checks if time was set by DCF in the last 48h or on sundays at three in the night (daylight saving changes)
//inline bool TimeWasSetRecently()
//{
//#ifdef USE_DCF77
//	int timeDifference = 72 + lastDcfSyncHour - HOUR;
//	if ( DOM < 3 )
//		timeDifference -= 24 * ( DOM - lastDcfSyncDay );
//	if ( DOW == 6 && HOUR == 3 && timeDifference != 72 )
//		return false;
//	if ( timeDifference > 0 )
//		return true;
//	return false;
//#else
//	return true;
//#endif
//}
//
//inline bool DcfReceiveModeIsActive()
//{
//#ifdef USE_DCF77
//	return !( ( minuteShiftRegContent & BIT_POSITION_ZWOELF ) || hourShiftRegContentLow || hourShiftRegContentHigh );
//#else
//	return false;
//#endif
//}
//
//inline void SetNormalTopLeftLedState()
//{
//	// Reset normal top left led status based on time
//	if( MIN % 5 > 3 )
//		IOSET |= ( 1 << LED_MIN_DOT4 );
//	else
//		IOCLR |= ( 1 << LED_MIN_DOT4 );
//}
//
//inline void ResumeNormalMode()
//{
//#ifndef DEBUG_SPACE_SAVING
//	specialModeEntrySecond = -1;
//	nPowerOnPressesInDcfSyncMode = 0;
//	currentClockMode = normal;
//	if ( ledEsIstIsOn )
//		IOSET |= ( 1 << LED_ES_IST );
//#endif	//DEBUG_SPACE_SAVING
//}
//
