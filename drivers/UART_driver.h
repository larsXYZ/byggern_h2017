/*
	UART DRIVER
    -Enables comm with pc via serial
    -Supports ATmega162 & ATmega2560
*/


#ifndef _UART_
#define _UART_

#include <avr/io.h>
#include <avr/interrupt.h>

//Functions
void UART_init(int baudrate);
void UART_send(unsigned char message);
unsigned char UART_receive();

#endif
