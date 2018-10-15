#include "esp8266.h"
#include <string.h>
#include <stdio.h>
#include "ub_lib/stm32_ub_dig_out.h"
#include "ub_lib/stm32_ub_systick.h"
#include "ub_lib/stm32_ub_uart.h"
#include <stm32_basicdefines.h>

Operation op;
Type t;

Esp8266_Ip_Status Esp8266IpStatus = ESP8266_IP_INIT;
Esp8266_Time_Status Esp8266TimeStatus = ESP8266_TIME_STATUS_INIT;

RTC_t Esp8266_curTime;

char receive[500] = {0};

const char ok_answer[] = "OK\r\n";
const size_t ok_answer_length = strlen(ok_answer);

const char wifi_connect_answer[] = "WIFI CONNECTED\r\n";
const char wifi_ip_ready_answer[] = "WIFI GOT IP\r\n";

const char at_inquiry[] = "AT";

void esp8266_init()
{
	UB_DigOut_Hi(DOUT_PB3);

	/*
	// Uncomment to set the module into firmware update mode
	UB_DigOut_Lo(DOUT_PB4);
	UB_DigOut_Lo(DOUT_PB5);
	UB_Systick_Pause_ms(10);
	UB_DigOut_Hi(DOUT_PB5);
	UB_Systick_Pause_ms(10);
	//UB_DigOut_Hi(DOUT_PB4);

	while(1);
	// Firmware Update
	*/

	UB_DigOut_Hi(DOUT_PB4);
	UB_DigOut_Lo(DOUT_PB5);

	UB_Systick_Pause_ms(10);

	UB_DigOut_Hi(DOUT_PB5);

	UB_Systick_Pause_ms(100);

	//while(1);


	int ready_statemachine = 0;
	do{
		char rx[500] = {0};
		int pop = UB_Uart_GetRXSize(COM2);//CircularBuffer_GetPopulation(&esp_rx_circ_buffer);

		pop = MIN(pop,500);

		UART_RXSTATUS_t ret = UB_Uart_ReadRxBuffer(COM2, rx, pop);//)CircularBuffer_Read(&esp_rx_circ_buffer, rx, pop);

		if( ret == RESULT_SUCCESS ) {
			int i=0;
			for( i=0; i<pop; ++i ) {
				if(rx[i] == 'i') {
					ready_statemachine = 1;
				}
				else if( ready_statemachine == 1 && rx[i] == 'n') {
					ready_statemachine = 2;
				}
				else if( ready_statemachine == 2 && rx[i] == 'v') {
					ready_statemachine = 3;
				}
				else if( ready_statemachine == 3 && rx[i] == 'a') {
					ready_statemachine = 4;
				}
				else if( ready_statemachine == 4 && rx[i] == 'l') {
					ready_statemachine = 5;
				}
			}
			if( ready_statemachine == 5 ) {
				// read id\r\n
				while(UB_Uart_GetRXSize(COM2) < 4);

				UB_Uart_ReadRxBuffer(COM2, rx, 4);
				break;
			}
		}
	}while(1);

	esp8266_send_command(INQUIRY, AT);

	UB_Systick_Pause_ms(10);

	UB_Uart_ReadLine(COM2, receive, 500);


	/*
	// Join WiFi AP
	UB_Uart_SendString(COM2, "AT+CWJAP=\"MKLan\",\"HumpaHumpaHumpaTaeterae\"", CRLF);
	UB_Systick_Pause_ms(50);
	UB_Uart_ReadLine(COM2, receive, 500);

	while(1);
	*/

	Esp8266_curTime.status = RTC_UNDEFINED;

}

void esp8266_send_command(Type type, Operation operation) {
    switch (operation) {
    case AT:
        op = AT;
        t = INQUIRY;
        UB_Uart_SendString(COM2, at_inquiry, CRLF);
        UB_Systick_Pause_ms(50);

        UB_Uart_ReadLine(COM2, receive, 500);

        UB_Uart_ReadLine(COM2, receive, 500);

        UB_Uart_ReadLine(COM2, receive, 500);
    }
}

void esp8266_wait_for_line_receive()
{
	int ret = -1;
	do {
		ret = UB_Uart_Is_Full_Line_Received(COM2);
	}while(ret == 0);
}

void esp8266_handle_receive()
{
	UB_Uart_ReadLine(COM2, receive, 500);

	if( strncmp(receive, wifi_connect_answer, strlen(wifi_connect_answer)) == 0 ) {
		int i;
	}
	else if( strncmp(receive, wifi_ip_ready_answer, strlen(wifi_ip_ready_answer)) == 0 ) {
		Esp8266IpStatus = ESP8266_IP_READY;
	}
}

int esp8266_request_time_from_google()
{
	int ret = 0;
	char strSend[500] = {0};

	if( Esp8266IpStatus != ESP8266_IP_READY ) return 0;

	if( Esp8266TimeStatus == ESP8266_TIME_STATUS_READY ) return 0;

	char strCIPStart[] = "AT+CIPSTART=\"TCP\",\"google.com\",80";
	char strCIPSend[] = "AT+CIPSEND=";
	char strCIPRequest[] = "HEAD / HTTP/1.1\r\nHost: google.com\r\n\r\n\r\n";

	UB_Uart_SendString(COM2, strCIPStart, CRLF);

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500);

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500);

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500);

	sprintf(strSend, "AT+CIPSEND=%d",strlen(strCIPRequest));
	UB_Uart_SendString(COM2, strSend, CRLF);

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500);

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500);

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500);

	UB_Uart_SendString(COM2, strCIPRequest, CRLF);

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500);

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500);

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); //busy...\r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // \r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // Recv... bytes \r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // \r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // Send OK\r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // \r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // +IPD, len...\r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // Cache control... \r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // Content type...\r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // Location...\r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // Content length...\r\n

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // Date: ....\r\n


	sscanf(&(receive[23]), "%d:%d:%d", &(Esp8266_curTime.std), &(Esp8266_curTime.min), &(Esp8266_curTime.sek));
	sscanf(&(receive[11]), "%d", &(Esp8266_curTime.tag));
	sscanf(&(receive[20]), "%d", &(Esp8266_curTime.jahr));

	if( strncmp(&(receive[6]), "Sun", 3) == 0 ) {
		Esp8266_curTime.wotag = 7;
	} else if( strncmp(&(receive[6]), "Mon", 3) == 0 ) {
		Esp8266_curTime.wotag = 1;
	} else if( strncmp(&(receive[6]), "Tue", 3) == 0 ) {
		Esp8266_curTime.wotag = 2;
	} else if( strncmp(&(receive[6]), "Wed", 3) == 0 ) {
		Esp8266_curTime.wotag = 3;
	} else if( strncmp(&(receive[6]), "Thu", 3) == 0 ) {
		Esp8266_curTime.wotag = 4;
	} else if( strncmp(&(receive[6]), "Fri", 3) == 0 ) {
		Esp8266_curTime.wotag = 5;
	} else if( strncmp(&(receive[6]), "Sat", 3) == 0 ) {
		Esp8266_curTime.wotag = 6;
	}

	if( strncmp(&(receive[14]), "Jan", 3) == 0) {
		Esp8266_curTime.monat = 1;
	} else if(strncmp(&(receive[14]), "Feb", 3) == 0) {
		Esp8266_curTime.monat = 2;
	} else if(strncmp(&(receive[14]), "Mar", 3) == 0) {
		Esp8266_curTime.monat = 3;
	} else if(strncmp(&(receive[14]), "Apr", 3) == 0) {
		Esp8266_curTime.monat = 4;
	} else if(strncmp(&(receive[14]), "May", 3) == 0) {
		Esp8266_curTime.monat = 5;
	} else if(strncmp(&(receive[14]), "Jun", 3) == 0) {
		Esp8266_curTime.monat = 6;
	} else if(strncmp(&(receive[14]), "Jul", 3) == 0) {
		Esp8266_curTime.monat = 7;
	} else if(strncmp(&(receive[14]), "Aug", 3) == 0) {
		Esp8266_curTime.monat = 8;
	} else if(strncmp(&(receive[14]), "Sep", 3) == 0) {
		Esp8266_curTime.monat = 9;
	} else if(strncmp(&(receive[14]), "Oct", 3) == 0) {
		Esp8266_curTime.monat = 10;
	} else if(strncmp(&(receive[14]), "Nov", 3) == 0) {
		Esp8266_curTime.monat = 11;
	} else if(strncmp(&(receive[14]), "Dec", 3) == 0) {
		Esp8266_curTime.monat = 12;
	}

	Esp8266_curTime = UB_RTC_CalculateTimeFromGmtTime(Esp8266_curTime);

	Esp8266_curTime.status = RTC_TIME_OK;

	esp8266_wait_for_line_receive();
	UB_Uart_ReadLine(COM2, receive, 500); // \r\n

	Esp8266TimeStatus = ESP8266_TIME_STATUS_READY;

	return 1;
}

// string - incoming buffer
u8 parse_AT(volatile char *buffer) {
    volatile char *pos = strstr(buffer, ok_answer);
    if (pos == 0) {
        return 1; // No match
    }

    u8 new_length = strlen(pos) + ok_answer_length;
    buffer = (volatile char*) memmove(buffer, pos, new_length);
    //usart_clear_string();

    return 0;
}

void esp8266_parse_line() {
	//UB_Uart_ReceiveString(COM2, receive);

    switch (op) {
    case AT:
        if (parse_AT(receive) != 0) {
            //usart2_print("-"); // error
        } else {
            //usart2_print("+"); // success
        }
    }
}
