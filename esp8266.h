#ifndef __ESP8266_H__
#define __ESP8266_H__
#include <stm32f4xx.h>
#include "ub_lib/stm32_ub_rtc.h"

typedef enum {
    AT_CWLAP, // Get AP list
    AT_CIFSR, // Get IP address
    AT_CWJAP,  // Connected AP info
    AT_CWQAP, // Disconnect from AP
    AT_CIPSERVER, // TCP Server
    AT // Check module presence
} Operation;

typedef enum {
    SET_EXECUTE,
    INQUIRY
} Type;

typedef enum {
    STATUS_WORKING,
    STATUS_NOT_WORKING,
    STATUS_PROBLEM,
} Status;

typedef enum {
	ESP8266_IP_INIT,
	ESP8266_IP_READY
} Esp8266_Ip_Status;

typedef enum {
	ESP8266_TIME_STATUS_INIT,
	ESP8266_TIME_STATUS_READY
} Esp8266_Time_Status;

extern Esp8266_Time_Status Esp8266TimeStatus;
extern RTC_t Esp8266_curTime;

void esp8266_init();
void esp8266_send_command(Type type, Operation operation);
void esp8266_parse_line();
void esp8266_handle_receive();

// returns 1 if the time was received, otherwise 0
int esp8266_request_time_from_google();

void esp8266_wait_for_line_receive();

#endif
