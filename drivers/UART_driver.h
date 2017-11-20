/*
	UART DRIVER
    -Allows communication with PC via serial
    -Supports ATMEGA162 & ATMEGA2560
*/


#ifndef _UART_
#define _UART_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//Functions
void UART_init(int baudrate);
void UART_send(unsigned char message);
unsigned char UART_receive();

#endif
