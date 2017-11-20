#include "SPI_driver.h"
#include <avr/io.h>
#include "MCP2515.h"

//Definitions of SPI pins
#ifdef  __AVR_ATmega162__

#define SPI_SS PB4
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK PB7

#elif __AVR_ATmega2560__

#define SPI_SS PB7
#define SPI_MOSI PB2
#define SPI_MISO PB3
#define SPI_SCK PB1

#endif

void SPI_init()
{
	
	//Sets SPI_MISO as output
	DDRB |= (1<<SPI_MOSI);
	
	//Sets SPI_SCK as output
	DDRB |= (1<<SPI_SCK);
	
	//Sets SPI_SS as output
	DDRB |= (1<<SPI_SS);
	
	//Makes the ATmega2560 happy, so we can use the SPI mode
	#ifdef __AVR_ATmega2560__
	DDRB |= (1<<PB0); 
	#endif
	
	//Sets SPI_MISO as input
	DDRB &= ~(1<<SPI_MISO);
	
	//Enables master mode
	SPCR |= (1<<MSTR);

	//Sets leading and trailing edge config, Sets transmittion rate, MCP2515 wants under 10 MHz -> f_osc / 64 is fine
	SPCR |= (0<<CPOL) | (0<<CPHA) | (1<<SPR1);
	
	//Enables SPI
	SPCR |= (1<<SPE);
}

int SPI_send_byte(uint8_t message)
{
	//Puts message in SPI DATA REGISTER
	SPDR = message;
	
	//Waits for transmission to complete
	while (!(SPSR & (1<<SPIF)));
	
	return 0;
}

int SPI_receive_byte()
{
	//Dummy data
	SPI_send_byte(0xFF);
	
	//Waits for transmission to complete
	while(!(SPSR & (1<<SPIF)));
	
	//Returns whats in the data register
	return SPDR;
}

void SPI_select_slave(int mode)
{
	if (mode == 1)
	{
		//Selects slave
		PORTB &= ~(1<<SPI_SS);
	}
	else if (mode == 0)
	{
		//Deselects slave
		PORTB |= (1<<SPI_SS);
	}
}

