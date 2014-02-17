// /***********************************************************************
// * main.cpp
// *
// *  Created on: 27.01.2011
// *      Author: Jan Berg
// *      Device: LPC2103 Microcontroller in Wordclock
// *      Purpose: Wordclock :)
// *
// ***********************************************************************/
//
////#include <math.h>
//#include <stdint.h>
//#include "lpc2103.h"
//#include "LPC2103_BasicHardware.h"
//
//#include "CommonMath.h"
//#include "LPC2103_RTC.h"
//#include "DCF77.h"
//#include "LPC2103_HsvToLedPwm.h"
//#include "irmp.h"
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
//struct rgbDuty
//{
//	float red;
//	float green;
//	float blue;
//};
//
//enum clockMode
//{
//	normal,	// setting changes apply to the words
//	//dcf77Sync,	//turn everything off
//	setTimeOffWeekday,
//	setTimeOnWeekday,
//	adjustAmbi,
//	testMode,
//	calibrationMode
//};
//
//void InitPinConfig( void );
//bool MoveBitpatternIntoShiftReg( const unsigned pinNoClock, const unsigned pinNoSerialIn, const char bitPattern );
//void SetWordMatrix( int hourToShow );
//void SetMinuteDotLeds();
//void ProcessIRdataPacket();
//bool TimeWasSetRecently();
//bool DcfReceiveModeIsActive();
//void SetNormalTopLeftLedState();
//void ResumeNormalMode();
//void AdjustBrightnessArray( bool increaseBrightness, float* brightnessMultiplierArray );
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
// *                      MAIN
// **********************************************************/
//
//int main( void )
//{
//	InitController( true );
//	InitPinConfig();
//
//	// Set up stuff for the time
//	InitRtc( false, PCLK );
//	rtcTime initialTime;
//	initialTime.year = 0;
//	initialTime.month = 0;
//	initialTime.dayOfWeek = 0;
//	initialTime.dayOfMonth = 0;
//	initialTime.hour = 0;
//#ifdef USE_DCF77
//	initialTime.minute = 0;
//	initialTime.second = 0;
//#else	// To enable an early wordmatrix adjustment
//	initialTime.minute = 4;
//	initialTime.second = 40;
//#endif
//	SetRtcTime( initialTime );
//
//	// DCF Stuff
//	Dcf77_Init();
//	IOSET |= ( 1 << PIN_DCF_PON );
//
//	// Initialize colors to white, turn of rotation and turn clock and ambilight on
//	colorClock.hue = 0.0;
//	colorClock.saturation = 0.0;
//	colorClock.value = 0.3;
//
//	colorAmbi.hue = 0.0;
//	colorAmbi.saturation = 0.0;
//	colorAmbi.value = 0.3;
//	colorRotationHueIncrementAmbi = 0.0;
//
//	// Activate PWMs and set them off initially
//	InitPwm( 800, PWM_CHANNELS );
//
//	SetPwmDutyCycle( PWM_CLOCK_RED_CH, 1.0 );
//	SetPwmDutyCycle( PWM_CLOCK_GREEN_CH, 1.0 );
//	SetPwmDutyCycle( PWM_CLOCK_BLUE_CH, 1.0 );
//
//	SetPwmDutyCycle( PWM_AMBI_RED_CH, 1.0 );
//	SetPwmDutyCycle( PWM_AMBI_GREEN_CH, 1.0 );
//	SetPwmDutyCycle( PWM_AMBI_BLUE_CH, 1.0 );
//
//	// Activate ADC burst mode for AIN2 (LDR). Does also set the pin configuration for this pin (P0.24)
//	//InitAdcBurstMode( 0x04 ); -> Replaced by manual calls
//	PINSEL1 |= ( 3 << 16 );
//	ADCR = 0x210D04;	// Equals ADCR_INIT | ( 1 << 16 ) | (1 << 2);
//
//	// Activate Interrupts
//	ActivateIRQ_Timer0( 10000, (void*) &TIMER0_ISR );
//	ActivateIRQ_Timer1( 100, (void*) &TIMER1_ISR );
//	ActivateIRQ_RTC( false, true, false, (void*) &RTC_INCREMENT_ISR );
//
//	// Indicate successfull booting
//	IOSET |= ( 1 << PIN_LED_OK );
//#ifndef USE_DCF77
//	SetWordMatrix( HOUR );
//#endif
//
//	unsigned lastCycleCounter100Hz = 0;
//	while( true )
//	{
//		// ### Routines that do not need exact timing -> here to reduce IRQ execution length
//		if ( CycleCounter100Hz != lastCycleCounter100Hz )
//		{
//			lastCycleCounter100Hz = CycleCounter100Hz;
//
//			// *********************************************************************************
//			// ****************** 	      Routines that run @ 10 Hz            *****************
//			// *********************************************************************************
//
//			// Measure and update ambient brightness
//#ifndef DEBUG_SPACE_SAVING
//			if ( CycleCounter100Hz %10 == 0 )
//				ambientBrightness = SlidingAverageOnLastValues( ComputeValueFromAdcRead( ADDR2, -1.0/1024.0, 1.0 ) );
//#endif	//DEBUG_SPACE_SAVING
//
//			// *********************************************************************************
//			// ****************** 	     Routines that run @ 100 Hz            ******************
//			// *********************************************************************************
//
//			// Perform color rotation if activated
//#ifndef DEBUG_SPACE_SAVING
//			colorClock.hue += colorRotationHueIncrementClock;
//			if ( colorClock.hue >= 360.0 )
//				colorClock.hue = colorRotationHueIncrementClock;
//			colorAmbi.hue += colorRotationHueIncrementAmbi;
//			if ( colorAmbi.hue >= 360.0 )
//				colorAmbi.hue = colorRotationHueIncrementAmbi;
//#endif	//DEBUG_SPACE_SAVING
//
//			// ##################################################################################
//			// ############################## Handle IR commands#################################
//			// ##################################################################################
//
//			IRMP_DATA irData;
//			if ( irmp_get_data( &irData ) )
//			{
//				currentIrData.address = irData.address;
//				currentIrData.command = irData.command;
//				currentIrData.flags = irData.flags;
//				currentIrData.protocol = irData.protocol;
//
//				// if a key is pressed, prolongue special function session
//				if ( specialModeEntrySecond != -1 )
//					specialModeEntrySecond = SEC;
//
//				// Handle long keypresses that enable special functions
//				if ( currentIrData.flags && lastIRCommandReceived == currentIrData.command )
//					++irCommandRepetitionCount;
//				else
//					irCommandRepetitionCount = 0;
//				lastIRCommandReceived = currentIrData.command;
//
//				ProcessIRdataPacket();
//			}
//
//			// As in the following the hue, saturation and value of clock/ambi is changed temporarily, save it first (writeback in the end)
//			float brightnessClock = colorClock.value;
//			float colorClockTempHue = colorClock.hue;
//			float colorClockTempSaturation = colorClock.saturation;
//			float brightnessAmbi = colorAmbi.value;
//
//			// #####################################################################################
//			// ##################### 		Automatic On/Off code 				####################
//			// #####################################################################################
//#ifndef DEBUG_SPACE_SAVING
//			// Times have changed or are adjusted. Show matrix in red/green
//			// Note: Setting of word matrix for showing currently selected time is ran @ 10 Hz
//			if ( autoOnOffIndicationCounter > 0 )
//			{
//				colorClock.saturation = 1.0;
//				// show off time
//				if ( autoOnOffIndicationCounter > 200 )
//				{
//					if ( autoOnOffIndicationCounter == 400 )
//						SetWordMatrix( autoOnOffWeekdayOffHour + 24 );	// Show only hours
//					colorClock.hue = 0.0;		//red
//				}
//				// show on time
//				else
//				{
//					if ( autoOnOffIndicationCounter == 200 )
//						SetWordMatrix( autoOnOffWeekdayOnHour + 24 );	// Show only hours
//					colorClock.hue = 120.0;	//green
//				}
//				--autoOnOffIndicationCounter;
//
//				// Show normal time again
//				if ( autoOnOffIndicationCounter == 0 )
//				{
//					SetWordMatrix( HOUR );
//					if ( ledEsIstIsOn )
//						IOSET |= ( 1 << LED_ES_IST );
//				}
//			}
//			// Adjust automatic on time
//			if ( currentClockMode == setTimeOffWeekday )
//			{
//				autoOnOffIndicationCounter = 0;
//				colorClock.saturation = 1.0;
//				colorClock.hue = 0.0;	//red
//				if ( autoOnOffSettingChanged )
//				{
//					SetWordMatrix( autoOnOffWeekdayOffHour + 24 );	// Show only hours
//					autoOnOffSettingChanged = false;
//				}
//			}
//			else if ( currentClockMode == setTimeOnWeekday )
//			{
//				autoOnOffIndicationCounter = 0;
//				colorClock.saturation = 1.0;
//				colorClock.hue = 120.0;	//green
//				if ( autoOnOffSettingChanged )
//				{
//					SetWordMatrix( autoOnOffWeekdayOnHour + 24 );	// Show only hours
//					autoOnOffSettingChanged = false;
//				}
//			}
//
//			// Switch clock off/on when time off/on weekdays is reached
//			// Note: When off/on time is the same, clock will stay on
//			if ( autoOnOffActive && IsWorkday() && currentClockMode == normal )
//			{
//				if ( autoOnOffWasSwitchedOn == false && HOUR == autoOnOffWeekdayOnHour )
//				{
//					clockIsOff = false;
//					autoOnOffWasSwitchedOn = true;
//					autoOnOffWasSwitchedOff = false;
//				}
//				if ( autoOnOffWasSwitchedOff == false && HOUR == autoOnOffWeekdayOffHour )
//				{
//					clockIsOff = true;
//					autoOnOffWasSwitchedOn = false;
//					autoOnOffWasSwitchedOff = true;
//				}
//			}
//			// Blinking when adjusting on/off times
//			if( ( currentClockMode == setTimeOnWeekday || currentClockMode == setTimeOffWeekday )&& CycleCounter100Hz < 20 )
//				colorClock.value = 0.0;
//#endif	//DEBUG_SPACE_SAVING
//
//			// ##################################################################################
//			// #################### Temporary Brightness/Color adjustments ######################
//			// ##################################################################################
//
//			// Adjust brightness levels to ambient light
//#ifndef DEBUG_SPACE_SAVING
//			colorClock.value *= LinearApproximation( ambientBrightnessPoints, ambientMultiplierPointsClock, 8, ambientBrightness );
//			if ( clockIsOff )
//				colorClock.value = 0.0;
//			colorAmbi.value *= LinearApproximation( ambientBrightnessPoints, ambientMultiplierPointsAmbi, 8, ambientBrightness );
//			if ( ambilightIsOff || clockIsOff )
//				colorAmbi.value = 0.0;
//
//			// ######## Dim word matrix during changes ######
//			if ( dimCounter >= 0 )
//			{
//					++dimCounter;
//				if ( dimCounter <= 100 * DIM_DURATION )	// Brightness down
//					colorClock.value *= 1.0 - dimCounter / ( 100.0 * DIM_DURATION );
//				if (  dimCounter == 100 * DIM_DURATION )
//				{
//					SetWordMatrix( HOUR );
//					SetMinuteDotLeds();
//				}
//				if ( dimCounter > 100 * DIM_DURATION )	// Brightness up again
//					colorClock.value *= ( dimCounter - 100 * DIM_DURATION ) / ( 100.0 * DIM_DURATION );
//				if ( dimCounter > 200 * DIM_DURATION )
//					dimCounter = -1;
//			}
//#else
//			SetWordMatrix( HOUR );
//#endif	//DEBUG_SPACE_SAVING
//
//			// Blinking when adjusting ambilight
//			if( currentClockMode == adjustAmbi && CycleCounter100Hz < 20 )
//				colorAmbi.value = 0.0;
//
//
//			// ##################################################################################
//			// ### Time needs to be fetched? Disable all PWMs and activate 4th min led in red ###
//			// ##################################################################################
//			if ( !TimeWasSetRecently() && !( currentClockMode == testMode || currentClockMode == calibrationMode ) )
//			{
//				// As PWM is inverted, only the red channel is active
//				// Todo: Setzen der hsv werte könnte ebenfalls funktionieren
//				SetPwmDutyCycle( PWM_CLOCK_RED_CH, 0.0 );
//				SetPwmDutyCycle( PWM_CLOCK_GREEN_CH, 1.0 );
//				SetPwmDutyCycle( PWM_CLOCK_BLUE_CH, 1.0 );
//				SetPwmDutyCycle( PWM_AMBI_RED_CH, 1.0 );
//				SetPwmDutyCycle( PWM_AMBI_GREEN_CH, 1.0 );
//				SetPwmDutyCycle( PWM_AMBI_BLUE_CH, 1.0 );
//
//				// Disable everything except top left led (but still allow IR data notification)
//				if ( lastIrDataRx100HzTime < 0 )
//					IOCLR |= 1 << LED_MIN_DOT3;
//				IOCLR |= ( 1 << LED_MIN_FUENF ) | ( 1 << LED_MIN_ZEHN ) | ( 1 << LED_MIN_ZWANZIG ) | ( 1 << LED_ES_IST )
//						 | ( 1 << LED_MIN_DOT1 ) | ( 1 << LED_MIN_DOT2 );
//				IOSET |= ( 1 << LED_MIN_DOT4 );
//
//				// Disable also word matrix (only once)
//				if ( !DcfReceiveModeIsActive() )
//				{
//					minuteShiftRegContent = 0x0;
//					hourShiftRegContentLow = 0x0;
//					hourShiftRegContentHigh = 0x0;
//					currentShiftRegToFill = 1;
//				}
//
//				IOCLR |= ( 1 << PIN_DCF_PON );	// Pull DCF77 PON low -> activate DCF module
//			}
//			// Set colors for all channels	(todo: only for normal mode?)
//			else if ( !( currentClockMode == testMode || currentClockMode == calibrationMode ) )
//			{
//				ConvertHSVtoPwmAndSet( colorClock, PWM_CLOCK_RED_CH, PWM_CLOCK_GREEN_CH, PWM_CLOCK_BLUE_CH );
//				ConvertHSVtoPwmAndSet( colorAmbi, PWM_AMBI_RED_CH, PWM_AMBI_GREEN_CH, PWM_AMBI_BLUE_CH );
//			}
//
//
//			// ##############################################################################
//			// ### Blink with top left minute led if color changing mode has been changed ###
//			// ##############################################################################
//			// Idea here: If mode has changed, led indication counter is set to 200 and decremented afterwards. The top
//			// left LED is then switched on and of depending on a modulo factor that is dependent on color rotation speed.
//			if ( colorRotationModeHasChanged || colorRotationLedIndicationCounter > 0 )
//			{
//#ifndef DEBUG_SPACE_SAVING
//				// choose blinking interval
//				float colorRotationIncrement = colorRotationHueIncrementClock;
//				if ( currentClockMode == adjustAmbi )
//					colorRotationIncrement = colorRotationHueIncrementAmbi;
//				// Blink three times in rapid or not so rapid fashion
//				int moduloValue = 300;	// do not blink
//				if ( colorRotationIncrement >= 0.05 )
//					moduloValue = 10;	//blink fast
//				else if ( colorRotationIncrement >= 0.005 )
//					moduloValue = 25;	//blink medium
//				else if ( colorRotationIncrement >= 0.0005 )
//					moduloValue = 66;	//blink slow
//
//				// handle blinking
//				if ( colorRotationLedIndicationCounter % moduloValue == 0 )
//				{
//					if( IOPIN & ( 1 << LED_MIN_DOT4 ) )
//						IOCLR |= ( 1 << LED_MIN_DOT4 );
//					else
//						IOSET |= ( 1 << LED_MIN_DOT4 );
//				}
//
//				// Handle led indication counter
//#endif	//DEBUG_SPACE_SAVING
//				--colorRotationLedIndicationCounter;
//#ifndef DEBUG_SPACE_SAVING
//				if ( colorRotationLedIndicationCounter <= 0 )
//				{
//					// Done with blinking, back to normal mode
//					if ( !colorRotationModeHasChanged )
//						SetNormalTopLeftLedState();
//					else
//					{
//						colorRotationLedIndicationCounter = 200;	//blinks over 2 seconds (as 100Hz)
//						colorRotationModeHasChanged = false;
//					}
//				}
//#endif	//DEBUG_SPACE_SAVING
//			}
//
//			// Write back hue, saturation and value for clock and ambilight
//			colorClock.value = brightnessClock;
//			colorClock.hue = colorClockTempHue;
//			colorClock.saturation = colorClockTempSaturation;
//			colorAmbi.value = brightnessAmbi;
//		}
//
//		// ##############################################################################
//		// ###############		 End of special modes and DCF 77 sync	 	#############
//		// ##############################################################################
//		// ### End special mode if no key was pressed for a SPECIAL_MODE_DURATION_AFTER_KEYPRESS seconds
//		if ( !( currentClockMode == normal || currentClockMode == testMode || currentClockMode == calibrationMode ) )
//		{
//			// Save Time at the beginning of the special mode
//			if ( specialModeEntrySecond == -1 )
//				specialModeEntrySecond = SEC;
//			// End special mode ( specialModeEntrySecond gets updated by ProcessIrDataPacket)
//			if ( SEC == ( ( specialModeEntrySecond + SPECIAL_MODE_DURATION_AFTER_KEYPRESS ) % 60 ) )
//			{
//				// ### Anything that needs to be restored before getting back to normal mode is dealt with below
//				if ( currentClockMode == setTimeOnWeekday || currentClockMode == setTimeOffWeekday )
//				{
//					SetWordMatrix( HOUR );
//					autoOnOffSettingChanged = true;	// Necessary in order to show set on/off time when adjusting
//				}
//
//				ResumeNormalMode();
//			}
//		}
//
//		// Show time instantly after it has been received
//		if ( TimeWasSetRecently() && DcfReceiveModeIsActive() )
//		{
//			SetWordMatrix( HOUR );
//			SetMinuteDotLeds();
//		}
//	}
//	return 0;
//}
//
///**********************************************************
// *                  Pin Configuration
// **********************************************************/
//// Note: Set your own configuration below
//void InitPinConfig( void )
//{
//	// Set GPIO Ports. Standard function is GPIO for others see user manual
//	// Note: To set bit N use |= ( 1 << N ), to delete it &= ~( 1 << N )
//	// Interesting registers: PINSEL0, PINSEL1 and IODIR
//
//	// set P0.6 as PIN_DCF_PON output pin (P0.6 is by default GPIO IN/PIN_DCF_DATA)
//	IODIR |= ( 1 << PIN_DCF_PON );
//
//	// Set PWM Outputs
//	IODIR |= ( 1 << PWM_CLOCK_RED_PIN );
//	IOSET |= ( 1 << PWM_CLOCK_RED_PIN );
//	IODIR |= ( 1 << PWM_CLOCK_GREEN_PIN );
//	IOSET |= ( 1 << PWM_CLOCK_GREEN_PIN );
//	IODIR |= ( 1 << PWM_CLOCK_BLUE_PIN );
//	IOSET |= ( 1 << PWM_CLOCK_BLUE_PIN );
//
//	// Set GPIO Outputs to output for OK Led, LEDs and shift registers
//	IODIR |= ( 1 << PIN_LED_OK );
//
//	IODIR |= ( 1 << LED_MIN_DOT1 );
//	IODIR |= ( 1 << LED_MIN_DOT2 );
//	IODIR |= ( 1 << LED_MIN_DOT3 );
//	IODIR |= ( 1 << LED_MIN_DOT4 );
//
//	IODIR |= ( 1 << LED_ES_IST );
//	IODIR |= ( 1 << LED_MIN_FUENF );
//	IODIR |= ( 1 << LED_MIN_ZEHN );
//	IODIR |= ( 1 << LED_MIN_ZWANZIG );
//
//	IODIR |= ( 1 << SHIFT_MINS_CLK );
//	IODIR |= ( 1 << SHIFT_MINS_IN );
//
//	IODIR |= ( 1 << SHIFT_HOURS_CLK );
//	IODIR |= ( 1 << SHIFT_HOURS_IN );
//
//	// Note: There is also some automatic pin configuration for the external
//	//		 interrupt, PWM ports and ADC burst channels.
//	// 		 For these ports, user settings above may be overwritten!
//}
//
///**********************************************************
// *                Function implementations
// **********************************************************/
//// Shifts the bit pattern into a shift register. Function must be called iteratively. If a shift completes
//// the return value is true and a new shift can be initiated.
//// Note: Function should only be called, when shifts are necessary
//bool MoveBitpatternIntoShiftReg( const unsigned pinNoClock, const unsigned pinNoSerialIn, const char bitPattern )
//{
//#ifndef DEBUG_SPACE_SAVING
//	if ( shiftClockIsHigh )
//	{
//		// Clear clock. If serial data was written, end
//		IOCLR |= ( 1 << pinNoClock );
//		if ( shiftBitNumber >= 8 )
//		{
//			shiftBitNumber = 0;
//			return true;
//		}
//
//		// Clock is not high -> Set serial in pin (pinstatus is used when clock is high again)
//		IOCLR |= ( 1 << pinNoSerialIn );
//		if ( bitPattern & ( 0x80 >> shiftBitNumber ) )	// MSB is used first
//			IOSET |= ( 1 << pinNoSerialIn );
//
//		++shiftBitNumber;
//	}
//	// Set clock high (input data will be used by the register and outputs set)
//	else
//		IOSET |= ( 1 << pinNoClock );
//
//	shiftClockIsHigh = !shiftClockIsHigh;
//#endif	//DEBUG_SPACE_SAVING
//	return false;
//}
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
//// In this function the IR data is used to change settings depending on the current clock mode
//// (e.g. for ambilight settings) and keypress duration -> function is called when key is released
//// Note: Uses currentIrData and irCommandRepetitionCount
//void ProcessIRdataPacket()
//{
//	// Process packet only if it came from the right remote
//	if ( firstSeenIrRemoteAddress == -1 )
//		firstSeenIrRemoteAddress = currentIrData.address;
//	else if ( firstSeenIrRemoteAddress != currentIrData.address )
//		return;
//
//	// Accept IR commands only when clock is on and not in dcf rx mode (except on button of course)
//#ifndef USE_ALTERNATIVE_IR_REMOTE
//	if ( ( clockIsOff || DcfReceiveModeIsActive() ) && currentIrData.command != 3 )
//#else
//	if ( ( clockIsOff || DcfReceiveModeIsActive() ) && currentIrData.command != 7 )
//#endif
//		return;
//
//	// Use bottom left LED to indicate received IR data
//	bool minLed3IsOn = IOPIN & ( 1 << LED_MIN_DOT3 );
//	if ( minLed3IsOn && lastIrDataRx100HzTime == -1 )
//	{
//		IOCLR |= ( 1 << LED_MIN_DOT3 );
//		lastIrDataRx100HzTime = CycleCounter100Hz;
//	}
//	else
//	{
//		IOSET |= ( 1 << LED_MIN_DOT3 );
//		lastIrDataRx100HzTime = CycleCounter100Hz;
//	}
//
//#ifndef DEBUG_SPACE_SAVING
//	// Color changes are saved temporarily and are used later depending on the clock mode
//	float hsvHueToSet = 0;
//	float hsvSaturationToSet = 1.0;
//
//	switch ( currentIrData.command )
//	{
//	// ##### Handle special buttons
//#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 0:	// Brightness +
//#else
//	case 5:
//#endif
//		if ( currentClockMode == normal && !clockIsOff )
//			AdjustBrightnessArray( true, ambientMultiplierPointsClock );
//		else if( currentClockMode == adjustAmbi && !ambilightIsOff && !clockIsOff )
//			AdjustBrightnessArray( true, ambientMultiplierPointsAmbi );
//		break;
//#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 1:	// Brightness -
//#else
//	case 4:
//#endif
//		if ( currentClockMode == normal && !clockIsOff )
//			AdjustBrightnessArray( false, ambientMultiplierPointsClock );
//		else if( currentClockMode == adjustAmbi && !ambilightIsOff && !clockIsOff )
//			AdjustBrightnessArray( false, ambientMultiplierPointsAmbi );
//		break;
//#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 2:	// OFF Button
//#else
//	case 6:
//#endif
//		if ( currentIrData.flags == 0 )
//		{
//			if ( currentClockMode == normal )
//			{
//				if ( autoOnOffIndicationCounter > 0 )
//					currentClockMode = setTimeOffWeekday;
//				else
//					clockIsOff = true;
//			}
//			else if( currentClockMode == adjustAmbi )
//				ambilightIsOff = true;
//			// Pushing on button while in autoOnOff adjustment mode decreases on time
//			else if ( currentClockMode == setTimeOnWeekday )
//			{
//				--autoOnOffWeekdayOnHour;
//				if ( autoOnOffWeekdayOnHour < 0 )
//					autoOnOffWeekdayOnHour = 23;
//				autoOnOffSettingChanged = true;
//			}
//			else if ( currentClockMode == setTimeOffWeekday )
//			{
//				--autoOnOffWeekdayOffHour;
//				if ( autoOnOffWeekdayOffHour < 0 )
//					autoOnOffWeekdayOffHour = 23;
//				autoOnOffSettingChanged = true;
//			}
//		}
//		break;
//#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 3:	// ON Button
//#else
//	case 7:
//#endif
//		if ( currentIrData.flags == 0 )
//		{
//			// Enable test mode when power on is pressed often during dcfSync
//			if ( DcfReceiveModeIsActive() )
//			{
//				++nPowerOnPressesInDcfSyncMode;
//				if ( nPowerOnPressesInDcfSyncMode >= N_POWER_ON_PRESS_TO_IGNORE_DCFSYNC_AND_ENABLE_TESTMODE )
//				{
//					currentClockMode = testMode;
//					// Set 10% duty on blue channels initially in test mode
//					SetPwmDutyCycle( PWM_CLOCK_RED_CH, 1.0 );
//					SetPwmDutyCycle( PWM_CLOCK_GREEN_CH, 1.0 );
//					SetPwmDutyCycle( PWM_CLOCK_BLUE_CH, 0.9 );
//					SetPwmDutyCycle( PWM_AMBI_RED_CH, 1.0 );
//					SetPwmDutyCycle( PWM_AMBI_GREEN_CH, 1.0 );
//					SetPwmDutyCycle( PWM_AMBI_BLUE_CH, 0.9 );
//				}
//			}
//			if ( currentClockMode == normal )
//			{
//				if ( autoOnOffIndicationCounter > 0 )
//					currentClockMode = setTimeOnWeekday;
//				else
//					clockIsOff = false;
//			}
//			else if( currentClockMode == adjustAmbi )
//				ambilightIsOff = false;
//			// Pushing on button while in autoOnOff adjustment mode increases on time
//			else if ( currentClockMode == setTimeOnWeekday )
//			{
//				autoOnOffWeekdayOnHour = ( ++autoOnOffWeekdayOnHour ) % 24;
//				autoOnOffSettingChanged = true;
//			}
//			else if ( currentClockMode == setTimeOffWeekday )
//			{
//				autoOnOffWeekdayOffHour = ( ++autoOnOffWeekdayOffHour ) % 24;
//				autoOnOffSettingChanged = true;
//			}
//			// pushing the ON button in testMode increases the time by 3 minutes and one hour and displays it
//			else if ( currentClockMode == testMode )
//			{
//				MIN = (MIN + 3) % 60;
//				HOUR = (HOUR + 1) % 24;
//				SetWordMatrix( HOUR );
//				SetMinuteDotLeds();
//				nPowerOnPressesInDcfSyncMode = 0;
//			}
//		}
//		break;
//
//#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 11:	// Flash button -> Enable/Disable ES IST
//#else
//	case 15:
//#endif
//		if ( currentIrData.flags == 0 )
//		{
//			ledEsIstIsOn = !ledEsIstIsOn;
//			if ( ledEsIstIsOn )
//				IOSET |= ( 1 << LED_ES_IST );
//			else
//				IOCLR |= ( 1 << LED_ES_IST );
//		}
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 15: // Strobe button -> Ambilight settings
//	#else
//	case 23:
//	#endif
//		currentClockMode = adjustAmbi;
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 19: // Fade button -> Fade through colors
//	#else
//	case 27:
//	#endif
//		if ( currentIrData.flags == 0 )
//		{
//			if ( currentClockMode == normal )
//			{
//				colorClock.saturation = 1.0;
//				if ( colorRotationHueIncrementClock == 0.0 )
//					colorRotationHueIncrementClock = 0.1;
//				else if ( colorRotationHueIncrementClock >= 0.05 )
//					colorRotationHueIncrementClock = 0.01;
//				else if ( colorRotationHueIncrementClock >= 0.005 )
//					colorRotationHueIncrementClock = 0.001;
//				else
//					colorRotationHueIncrementClock = 0.0;
//				colorRotationModeHasChanged = true;
//			}
//			else if( currentClockMode == adjustAmbi )
//			{
//				colorAmbi.saturation = 1.0;
//				if ( colorRotationHueIncrementAmbi == 0.0 )
//					colorRotationHueIncrementAmbi = 0.1;
//				else if ( colorRotationHueIncrementAmbi >= 0.05 )
//					colorRotationHueIncrementAmbi = 0.01;
//				else if ( colorRotationHueIncrementAmbi >= 0.005 )
//					colorRotationHueIncrementAmbi = 0.001;
//				else
//					colorRotationHueIncrementAmbi = 0.0;
//				colorRotationModeHasChanged = true;
//			}
//			// Fade button enables calibration mode when in test mode
//			// This sets the color to white and cycles through the brightnesses in the brightness array with each additional press of the fade button
//			// The currently active index is indicated by the word matrix in hour (note: first index is 1 in display)
//			else if ( currentClockMode == testMode )
//			{
//				// Set and indicate index in brightness array
//				currentClockMode = calibrationMode;
//				MIN = 0;
//				HOUR = 1;
//				SetWordMatrix( HOUR );
//				SetMinuteDotLeds();
//
//				// Set white intensity for word matrix based on first calibration point
//				hsvColor color;
//				color.saturation = 0;	// white
//				color.value = colorCorrectDimmingPoints[0];	// set brightness
//				ConvertHSVtoPwmAndSet( color, PWM_CLOCK_RED_CH, PWM_CLOCK_GREEN_CH, PWM_CLOCK_BLUE_CH );
//
//				// Disable Ambilight
//				SetPwmDutyCycle( PWM_AMBI_RED_CH, 1.0 );
//				SetPwmDutyCycle( PWM_AMBI_GREEN_CH, 1.0 );
//				SetPwmDutyCycle( PWM_AMBI_BLUE_CH, 1.0 );
//			}
//			else if ( currentClockMode == calibrationMode )
//			{
//				// Set and indicate index in brightness array
//				int currentIdx = HOUR + 1;
//				if ( currentIdx >= N_COLOR_CORRECT_DIMMING_PTS )
//					currentIdx = 1;
//				HOUR = currentIdx;
//				SetWordMatrix( HOUR );
//				SetMinuteDotLeds();
//
//				// Set white intensity for word matrix based on current calibration point
//				hsvColor color;
//				color.saturation = 0;	// white
//				color.value = colorCorrectDimmingPoints[currentIdx-1];	// set brightness to current point
//				ConvertHSVtoPwmAndSet( color, PWM_CLOCK_RED_CH, PWM_CLOCK_GREEN_CH, PWM_CLOCK_BLUE_CH );
//			}
//		}
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 23:	// Smooth button -> Choose of/on times for clock
//	#else
//	case 19:
//	#endif
//		{
//			if ( currentClockMode == testMode || currentClockMode == calibrationMode )
//				ResumeNormalMode();	// By pressing the smooth button in test or calibration mode, normal operation is resumed
//			else
//			{
//				autoOnOffActive = !autoOnOffActive;
//				if ( autoOnOffActive )
//					autoOnOffIndicationCounter = 400;	// Starts display of automatic on/off times
//			}
//		}
//		break;
//
//	// ##### Handle color buttons
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 4:	// Color: Red
//	#else
//	case 9:
//	#endif
//		hsvHueToSet = 0.0;
//		if ( currentClockMode == testMode )
//		{
//			// Set 10% duty on red channels
//			SetPwmDutyCycle( PWM_CLOCK_RED_CH, 0.9 );
//			SetPwmDutyCycle( PWM_CLOCK_GREEN_CH, 1.0 );
//			SetPwmDutyCycle( PWM_CLOCK_BLUE_CH, 1.0 );
//			SetPwmDutyCycle( PWM_AMBI_RED_CH, 0.9 );
//			SetPwmDutyCycle( PWM_AMBI_GREEN_CH, 1.0 );
//			SetPwmDutyCycle( PWM_AMBI_BLUE_CH, 1.0 );
//		}
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 5:	// Color: Green
//	#else
//	case 8:
//	#endif
//		hsvHueToSet = 120;
//		if ( currentClockMode == testMode )
//		{
//			// Set 10% duty on green channels
//			SetPwmDutyCycle( PWM_CLOCK_RED_CH, 1.0 );
//			SetPwmDutyCycle( PWM_CLOCK_GREEN_CH, 0.9 );
//			SetPwmDutyCycle( PWM_CLOCK_BLUE_CH, 1.0 );
//			SetPwmDutyCycle( PWM_AMBI_RED_CH, 1.0 );
//			SetPwmDutyCycle( PWM_AMBI_GREEN_CH, 0.9 );
//			SetPwmDutyCycle( PWM_AMBI_BLUE_CH, 1.0 );
//		}
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 6:	// Color: Blue
//	#else
//	case 10:
//	#endif
//		hsvHueToSet = 240;
//		if ( currentClockMode == testMode )
//		{
//			// Set 10% duty on blue channels
//			SetPwmDutyCycle( PWM_CLOCK_RED_CH, 1.0 );
//			SetPwmDutyCycle( PWM_CLOCK_GREEN_CH, 1.0 );
//			SetPwmDutyCycle( PWM_CLOCK_BLUE_CH, 0.9 );
//			SetPwmDutyCycle( PWM_AMBI_RED_CH, 1.0 );
//			SetPwmDutyCycle( PWM_AMBI_GREEN_CH, 1.0 );
//			SetPwmDutyCycle( PWM_AMBI_BLUE_CH, 0.9 );
//		}
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 7:	// Color: White
//	#else
//	case 11:
//	#endif
//		hsvSaturationToSet = 0;
//		break;
//
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 8:	// Color: Orange
//	#else
//	case 13:
//	#endif
//		hsvHueToSet = 15;
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 9:	// Color: Lightgreen
//	#else
//	case 12:
//	#endif
//		hsvHueToSet = 140;
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 10:	// Color: Darkblue
//	#else
//	case 14:
//	#endif
//		hsvHueToSet = 260;
//		break;
//
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 12:	// Color: Lightorange
//	#else
//	case 21:
//	#endif
//		hsvHueToSet = 30;
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 13:	// Color: Light greenblue
//	#else
//	case 20:
//	#endif
//		hsvHueToSet = 160;
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 14:	// Color: Lila
//	#else
//	case 22:
//	#endif
//		hsvHueToSet = 280;
//		break;
//
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 16:	// Color: Darkyellow
//	#else
//	case 25:
//	#endif
//		hsvHueToSet = 45;
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 17:	// Color: Greenblue
//	#else
//	case 24:
//	#endif
//		hsvHueToSet = 180;
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 18:	// Color: Pink
//	#else
//	case 26:
//	#endif
//		hsvHueToSet = 310;
//		break;
//
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 20:	// Color: Yellow
//	#else
//	case 17:
//	#endif
//		hsvHueToSet = 60;
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 21:	// Color: Darkbluegreen
//	#else
//	case 16:
//	#endif
//		hsvHueToSet = 200;
//		break;
//	#ifndef USE_ALTERNATIVE_IR_REMOTE
//	case 22:	// Color: Darkpink
//	#else
//	case 18:
//	#endif
//		hsvHueToSet = 340;
//		break;
//	}
//
//	// Use settings depending on the clockmode only if no special button was pressed
//#ifndef USE_ALTERNATIVE_IR_REMOTE
//	if ( currentIrData.command >= 4 && currentIrData.command != 11 && currentIrData.command != 15 && currentIrData.command != 19 && currentIrData.command != 23 )
//#else
//	if ( currentIrData.command != 4 && currentIrData.command != 5 && currentIrData.command != 6 && currentIrData.command != 7
//		 &&	currentIrData.command != 15 && currentIrData.command != 23 && currentIrData.command != 27 && currentIrData.command != 19 )
//#endif
//	{
//		// Normal Mode: Use the color to set the clock
//		if ( currentClockMode == normal )
//		{
//			colorClock.hue = hsvHueToSet;
//			colorClock.saturation = hsvSaturationToSet;
//		}
//		if ( currentClockMode == adjustAmbi )
//		{
//			colorAmbi.hue = hsvHueToSet;
//			colorAmbi.saturation = hsvSaturationToSet;
//		}
//	}
//
//	// Reset irCommandRepetitionCount
//	if ( irCommandRepetitionCount > KEYPRESSES_COUNT_FOR_SPECIALACTION )
//		irCommandRepetitionCount = 0;
//
//#endif	//DEBUG_SPACE_SAVING
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
//
//// Chooses the ambient brightness depending on ambient to led brightness conversion array
//void AdjustBrightnessArray( bool increaseBrightness, float* brightnessMultiplierArray )
//{
//#ifndef DEBUG_SPACE_SAVING
//	// First: Find first index in ambientBrighntess array that holds a smaller brightness than the measured one
//	int closestAmbientBrightnessIdx;
//	// Note: Iteration starts at -2, as the highest idx, that is 1, will always be higher (and it ensures that no out of bounds access occurs later on)
//	for ( closestAmbientBrightnessIdx=N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-2; closestAmbientBrightnessIdx >= 0; --closestAmbientBrightnessIdx )
//		if ( ambientBrightnessPoints[closestAmbientBrightnessIdx] < ambientBrightness )
//			break;
//
//	// Second: Calculate relative difference to next higher brightness point. This indicates how much effect
//	// the brightness change has to the indicies with brightnesses inbetween the measured brightness
//	//	Note: Here it is necessary, that no two brightness points are the same
//	float adjustmentFactorHigherIdx = ( ambientBrightness - ambientBrightnessPoints[closestAmbientBrightnessIdx] ) /
//			( ambientBrightnessPoints[closestAmbientBrightnessIdx + 1 ] - ambientBrightnessPoints[closestAmbientBrightnessIdx] );
//
//	// Calculate effect on brightnesses and set them
//	// Also, limit brightnesses and adjust brightness levels beneath to ensure strict monotony
//	if ( increaseBrightness )
//	{
//		brightnessMultiplierArray[closestAmbientBrightnessIdx] += ( 1.0 - adjustmentFactorHigherIdx ) * ( BRIGHTNESS_DIM_FACTOR - 1.0 )* brightnessMultiplierArray[closestAmbientBrightnessIdx];
//		brightnessMultiplierArray[closestAmbientBrightnessIdx+1] += adjustmentFactorHigherIdx * ( BRIGHTNESS_DIM_FACTOR - 1.0 ) * brightnessMultiplierArray[closestAmbientBrightnessIdx+1];
//
//		// adjust higher brightlevels
//		// to reduce the complexity of the following loop, index zero is dealt with alone
//		brightnessMultiplierArray[0] = MINIMUM( brightnessMultiplierArray[0], 0.9 );
//		for( int i=1; i<N_AMBIENT_BRIGHTNESS_CONVERSION_PTS; ++i )
//			brightnessMultiplierArray[i] = MINIMUM( 1.0 - ( N_AMBIENT_BRIGHTNESS_CONVERSION_PTS - closestAmbientBrightnessIdx ) * MIN_BRIGHTNESS_MULTI_DIFF, //upper limit
//					MAXIMUM ( brightnessMultiplierArray[i-1] + MIN_BRIGHTNESS_MULTI_DIFF, brightnessMultiplierArray[i] ) );	// lower limit
//	}
//	else
//	{
//		brightnessMultiplierArray[closestAmbientBrightnessIdx] -= ( 1.0 - adjustmentFactorHigherIdx ) * ( 1.0 - 1.0 / BRIGHTNESS_DIM_FACTOR ) * brightnessMultiplierArray[closestAmbientBrightnessIdx];
//		brightnessMultiplierArray[closestAmbientBrightnessIdx+1] -= adjustmentFactorHigherIdx * ( 1.0 - 1.0 / BRIGHTNESS_DIM_FACTOR ) * brightnessMultiplierArray[closestAmbientBrightnessIdx+1];
//
//		// adjust lower brightlevels
//		// to reduce the complexity of the following loop, the highest index is dealt with alone
//		brightnessMultiplierArray[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-1] = MAXIMUM( brightnessMultiplierArray[N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-1], 0.0 );
//		for( int i=N_AMBIENT_BRIGHTNESS_CONVERSION_PTS-2; i>=0; --i )
//			brightnessMultiplierArray[i] = MAXIMUM( i * MIN_BRIGHTNESS_MULTI_DIFF, // lower limit
//					MINIMUM ( brightnessMultiplierArray[i+1] - MIN_BRIGHTNESS_MULTI_DIFF, brightnessMultiplierArray[i] ) );	// upper limit
//	}
//#endif	//DEBUG_SPACE_SAVING
//}
//
