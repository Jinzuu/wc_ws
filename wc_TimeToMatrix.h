#ifndef __WC_TIMETOMATRIX_H
#define __WC_TIMETOMATRIX_H

#include "wc_frontend.h"
#include "ub_lib/stm32_ub_rtc.h"
//####################################################################################################
//##################################### EXTERNAL FUNCTIONS ###########################################
//####################################################################################################

// Function uses rtc time to set the wc_frontend
void SetWordMatrix( RTC_t time );



#endif __WC_TIMETOMATRIX_H
