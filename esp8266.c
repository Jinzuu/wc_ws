#include "esp8266.h"
#include <string.h>
#include "ub_lib/stm32_ub_dig_out.h"
#include "ub_lib/stm32_ub_systick.h"
#include "ub_lib/stm32_ub_uart.h"

Operation op;
Type t;

char receive[500] = {0};

const char ok_answer[] = "OK\r\n";
const size_t ok_answer_length = strlen(ok_answer);

const char at_inquiry[] = "AT\r\n";

void esp8266_init()
{
	UB_DigOut_Lo(DOUT_PB5);

	UB_Systick_Pause_ms(10);

	UB_DigOut_Hi(DOUT_PB5);
	UB_DigOut_Hi(DOUT_PB3);
}

void esp8266_send_command(Type type, Operation operation) {

    switch (operation) {
    case AT:
        op = AT;
        t = INQUIRY;
        UB_Uart_SendString(COM2, at_inquiry, CRLF);
    }
}


// string - incoming buffer
u8 parse_AT(volatile char *buffer) {
    volatile char *pos = strstr(buffer, ok_answer);
    if (pos == 0) {
        return 1; // No match
    }

    u8 new_length = strlen(pos) + ok_answer_length;
    buffer = (volatile char*) memmove(buffer, pos, new_length);
    usart_clear_string();

    return 0;
}

void esp8266_parse_line() {
	UB_Uart_ReceiveString(COM2, receive);

    switch (op) {
    case AT:
        if (parse_AT(receive) != 0) {
            usart2_print("-"); // error
        } else {
            usart2_print("+"); // success
        }
    }
}
