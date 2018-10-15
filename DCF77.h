#ifndef _DCF77_H_
#define _DCF77_H_
/***********************************************************************
 * Functions for DCF77 Decoding
 *
 * Basically you just need to call DCF77_ProcessBit at least 100 times a
 * second (adjust defines if you change this).
 *
 * Jan Berg, 2011 (updated 2014 for use with STM32 and UB libs)
 ***********************************************************************/

#include <stm32f4xx_gpio.h>
#include "ub_lib/stm32_ub_rtc.h"


/**********************************************************
 *              Defines for Configuration
 **********************************************************/
// If defined, the time is only accepted if two subsequent DCF receptions differ exactly by a minute
#define ENABLE_DOUBLE_TIMECHECK
// Sampling frequency in Hz (number of calls of Dcf77_ProcessSignal per second). Set > 100
#define DCF77_SAMPLE_FREQUENCY	100

/**********************************************************
 *                      Enumerations
 **********************************************************/
typedef enum
{
	dcf77_RxStateUnkown,
	dcf77_RxStateGood,
	dcf77_TimeRxSuccess,
	dcf77_TimeRxFail
}dcf77_SignalState_t;

/**********************************************************
 *                      Functions
 **********************************************************/

// Returns 1 if the DCF signal was complete and the time can be read
// Note: It is expected that the input is usually low and only for 100 or 200ms high
dcf77_SignalState_t Dcf77_ProcessSignal( BitAction dcfInputState );

// Returns the last correctly received time
RTC_t Dcf77_GetTime();



#endif /* _DCF77_H_ */
