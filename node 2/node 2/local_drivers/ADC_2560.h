/*
	DRIVER FOR THE INTERNAL ADC ON THE ATMEGA2560
		- Uses analog pins A0 ->
*/

#ifndef _ADC2560
#define _ADC2560

#include <inttypes.h>

//Functions
void adc2560_init(); //Initializes the adc with single pin input
uint16_t adc2560_read_bin(int ch); //Read the binary voltage[0-1023] of the pin from channel, ch [0-7]
float adc2560_read_v(int ch); //Read the voltage[0.0-5.0] of the pin from channel, ch [0-7]


#endif