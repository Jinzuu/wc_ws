#include "DCF77.h"

/**********************************************************
 *               Further Configuration Defines
 **********************************************************/
// Defines the length of a sent zero in samples (=100ms)
#define DCF77_ZERO_LENGTH	10
// Defines the length of a sent one in samples (=200ms)
#define DCF77_ONE_LENGTH	20
// Defines, how many samples length differences will be still yield valid signals
#define DCF77_SAMPLE_TOLERANCE	3
// Defines after how many seconds that looked good, dcf77_RxStateGood is reported
#define DCF77_RX_STATE_GOOD_SECOND_THRESHOLD	10


/**********************************************************
 *                      Static variables
 **********************************************************/

// Saves the last seen state of the signal for transition detection
volatile uint8_t dcfSignalWasHigh = 0;
// The last eight inputs are saved for error correction
volatile uint8_t lastSignalsSeen = 0;
// Ticks (calls to the Dcf77_ProcessSignal) since the last transition
volatile int ticksSinceLastTransition = 0;
// Bits that were received in the current minute
// TODO: code size could be optimized by using two int's instead
volatile uint8_t receivedDcf77Bits[60] = {0};
// Current second counter (also index to receivedDcf77Bits)
// Note: If currentSecond is -1, the second is not synced
volatile int currentSecond = -1;
// Tick counter in order to increase the seconds
volatile int subSecondCounter = 0;
// Counter to save how many of the last rx bits looked ok
volatile int nLastBitsLookedOk = 0;

volatile RTC_t lastExtractedTime;

/**********************************************************
 *                      Support Stuff
 **********************************************************/

typedef enum
{
	pinIsHigh,
	pinIsLow,
	pinIsIndifferent
} dcfPinStatus_t;

// Convert a series of received dcf bits to an integer value
// Note: It is assumed that the length is between three and eight bits
int GetValueFromDcf77Bits( uint8_t* dcfData, unsigned dataLength )
{
	int value = dcfData[0];
	int valueToAddIfBitIsTrue = 1;
	int i;
	for ( i=1; i<dataLength; ++i )
	{
		valueToAddIfBitIsTrue *= 2;
		if ( i==4 )	// Take care of the BCD format
			valueToAddIfBitIsTrue = 10;

		if ( dcfData[i] )
			value += valueToAddIfBitIsTrue;
	}

	return value;
}


// This function processes the 60 bits extracted from the dcf77 signal during a minute
// It returns 1, if there were no errors and the time could be extracted
int ProcessDcf77Bits()
{
	// Check parity
	uint8_t startBitIsSet = receivedDcf77Bits[20];	// Check for startbit( should always be "1" or true for that matter )

	// Minute parity
	uint8_t minuteParityFits = !receivedDcf77Bits[28];
	int i = 0;
	for ( i=0; i<7; ++i )
		if ( receivedDcf77Bits[21+i] )
			minuteParityFits = !minuteParityFits;
	// Hour parity
	uint8_t hourParityFits = !receivedDcf77Bits[35];
	for ( i=0; i<6; ++i )
		if ( receivedDcf77Bits[29+i] )
			hourParityFits = !hourParityFits;
	// Date parity
	uint8_t dateParityFits = !receivedDcf77Bits[58];
	for ( i=0; i<22; ++i )
		if ( receivedDcf77Bits[36+i] )
			dateParityFits = !dateParityFits;

	// Get DCF Data
	if ( startBitIsSet && minuteParityFits && hourParityFits && dateParityFits )
	{
#ifdef ENABLE_DOUBLE_TIMECHECK
		uint8_t timeMatchesWithPreviousTime = 0;
		if ( ( lastExtractedTime.min + 1 == GetValueFromDcf77Bits( &( receivedDcf77Bits[21]), 7 ) )
				&& ( lastExtractedTime.std == GetValueFromDcf77Bits( &( receivedDcf77Bits[29]), 6 ) )
				&& ( lastExtractedTime.tag == GetValueFromDcf77Bits( &( receivedDcf77Bits[36]), 6 ) )
				&& ( lastExtractedTime.wotag == GetValueFromDcf77Bits( &( receivedDcf77Bits[42]), 3 ) )
				&& ( lastExtractedTime.monat  == GetValueFromDcf77Bits( &( receivedDcf77Bits[45]), 5 ) )
				&& ( lastExtractedTime.jahr == GetValueFromDcf77Bits( &( receivedDcf77Bits[50]), 8 ) ) )
			timeMatchesWithPreviousTime = 1;
#endif
		lastExtractedTime.sek = 0;
		lastExtractedTime.min = GetValueFromDcf77Bits( &( receivedDcf77Bits[21]), 7 );
		lastExtractedTime.std = GetValueFromDcf77Bits( &( receivedDcf77Bits[29]), 6 );
		lastExtractedTime.tag = GetValueFromDcf77Bits( &( receivedDcf77Bits[36]), 6 );
		lastExtractedTime.wotag = GetValueFromDcf77Bits( &( receivedDcf77Bits[42]), 3 );
		lastExtractedTime.monat  = GetValueFromDcf77Bits( &( receivedDcf77Bits[45]), 5 );
		lastExtractedTime.jahr = GetValueFromDcf77Bits( &( receivedDcf77Bits[50]), 8 );
#ifdef ENABLE_DOUBLE_TIMECHECK
		return timeMatchesWithPreviousTime;
#else
		return 1;
#endif
	}

	return 0;
}

/**********************************************************
 *                      Functions
 **********************************************************/

RTC_t Dcf77_GetTime()
{
	RTC_t time;
	time.jahr = lastExtractedTime.jahr;
	time.monat = lastExtractedTime.monat;
	time.tag = lastExtractedTime.tag;
	time.wotag = lastExtractedTime.wotag;
	time.std = lastExtractedTime.std;
	time.min = lastExtractedTime.min;
	time.sek = lastExtractedTime.sek;

	return time;
}

dcf77_SignalState_t Dcf77_ProcessSignal( BitAction dcfInputState )
{
	uint8_t receivedDcf77BitsAreComplete = 0;	// is set to 1 if a full signal has been processed

	// Increments with every call to this function
	++ticksSinceLastTransition;
	++subSecondCounter;
	if ( subSecondCounter >= DCF77_SAMPLE_FREQUENCY )
		subSecondCounter = 0;

	lastSignalsSeen <<= 1;
	if ( dcfInputState == Bit_SET )
		lastSignalsSeen |= 0x1;

	// Repair signal. Only two bits are allowed to be different
	dcfPinStatus_t currentInputPinStatus = pinIsIndifferent;
	char lastEightInputs = lastSignalsSeen;
	char numOfHighBits;
	for ( numOfHighBits = 0; lastEightInputs; lastEightInputs >>= 1 )
		numOfHighBits += lastEightInputs & 1;
	if ( numOfHighBits > 5 )
		currentInputPinStatus = pinIsHigh;
	else if ( numOfHighBits < 3 )
		currentInputPinStatus = pinIsLow;

	// Investigate transition type High To Low -> Length of High state?
	if ( currentInputPinStatus == pinIsLow && dcfSignalWasHigh == 1 )
	{
		// Store data only, when we are synced
		if ( currentSecond >= 0 )
		{
			// increment second on rising edge
			currentSecond = ( ++currentSecond ) % 59;

			unsigned short int lengthOfHighPhase = ticksSinceLastTransition;
			if ( lengthOfHighPhase >= DCF77_ZERO_LENGTH - DCF77_SAMPLE_TOLERANCE
					&& lengthOfHighPhase <= DCF77_ZERO_LENGTH + DCF77_SAMPLE_TOLERANCE ){
				receivedDcf77Bits[currentSecond] = 0;
				++nLastBitsLookedOk;
			}
			else if ( lengthOfHighPhase >= DCF77_ONE_LENGTH - DCF77_SAMPLE_TOLERANCE
					&& lengthOfHighPhase <= DCF77_ONE_LENGTH + DCF77_SAMPLE_TOLERANCE ){
				receivedDcf77Bits[currentSecond] = 1;
				++nLastBitsLookedOk;
			}
			else	// signal length did not fit
				nLastBitsLookedOk = 0;
			//	Dcf77_Init();
		}

		ticksSinceLastTransition = 0;
		dcfSignalWasHigh = 0;
	}
	// Investigate transition type low to high
	else if ( currentInputPinStatus == pinIsHigh && dcfSignalWasHigh == 0 )
	{
		unsigned short lengthOfLowPhase = ticksSinceLastTransition;

		// Signal in 59th second: None for a limited amount of time
		// Todo: Leap seconds are not processed correctly
		if( lengthOfLowPhase >= DCF77_SAMPLE_FREQUENCY
				&& lengthOfLowPhase <= DCF77_SAMPLE_FREQUENCY << 2 )
		{
			// Read time is only accepted, if the timing is more or less accurate
			if ( currentSecond == 58 )
				receivedDcf77BitsAreComplete = 1;
			currentSecond = 58;
			subSecondCounter = DCF77_SAMPLE_FREQUENCY - 1;	// Sync on rising dcf edge
			++nLastBitsLookedOk;
		}
		//else	// Length of zero interval too short or long
		//	Dcf77_Init();

		ticksSinceLastTransition = 0;
		dcfSignalWasHigh = 1;
	}

	// If a a full minute of dcf signal was read, process it and extract the time
	if ( receivedDcf77BitsAreComplete == 1 ){
		if ( ProcessDcf77Bits() == 1 )
			return dcf77_TimeRxSuccess;
		return dcf77_TimeRxFail;
	}

	// Prevent wrap around
	if ( nLastBitsLookedOk > DCF77_RX_STATE_GOOD_SECOND_THRESHOLD*4 )
		nLastBitsLookedOk = DCF77_RX_STATE_GOOD_SECOND_THRESHOLD;

	if ( nLastBitsLookedOk >= DCF77_RX_STATE_GOOD_SECOND_THRESHOLD )
		return dcf77_RxStateGood;
	return dcf77_RxStateUnkown;
}


