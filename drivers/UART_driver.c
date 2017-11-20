#include "UART_driver.h"
#include <stdio.h>
#include "f_cpu.h"

static FILE usart_stdout = FDEV_SETUP_STREAM(UART_send, NULL, _FDEV_SETUP_WRITE);

void UART_init(int baudrate)
{
	uint16_t MYUBRR = (F_CPU/16/baudrate)-1;
	
	//Setting baud rate
	UBRR0H = (unsigned char)(MYUBRR >>8);
	UBRR0L = (unsigned char)(MYUBRR );
	
	//Enabling interrupts
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	//Setting frame format
	#ifdef __AVR_ATmega162__
	UCSR0C = (1<<URSEL0) | (1<<UCSZ00) | (1<<UCSZ01);
	#elif __AVR_ATmega2560__
	UCSR0C = (3<<UCSZ00);
	#endif
	
	//Links prinf() to USART output
	stdout = &usart_stdout;
}

void UART_send(unsigned char message)
{
	//Waiting for data buffer to be ready
	while ( !( UCSR0A & (1<<UDRE0)) ){;}
	
	//Fills data buffer
	UDR0 = message;
}

unsigned char UART_receive()
{
	//Waiting for data buffer to receive data
	while ( !(UCSR0A & (1<<RXC0)) ){;}
	
	return UDR0;
}

