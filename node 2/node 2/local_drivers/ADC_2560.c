#include "ADC_2560.h"
#include <avr/io.h>
#include <stdio.h>

//See .h file for function explanations

void adc2560_init()
{
	//Enable ADC
	ADCSRA |= (1 << ADEN);

	//Set clock prescaler @128
	ADCSRA |= (3 << ADPS0);
	
	ADMUX |= (1 << REFS0);

}

uint16_t adc2560_read_bin(int ch)
{
	//Select Channel
	ADMUX |= (0x1F & ch);
	
	//Start conversion
	ADCSRA |= (1 << ADSC);
	
	//Wait for result
	while((1 << ADSC) & ADCSRA){;}
	
	//Read result
	uint16_t voltage = ADC;
	
	return voltage;
}

float adc2560_read_v(int ch)
{
		//Select Channel
	ADMUX |= (0x1F & ch);
	
	//Start conversion
	ADCSRA |= (1 << ADSC);
	
	//Wait for result
	while((1 << ADSC) & ADCSRA){;}
	
	//Read result
	uint16_t voltage = ADC;
	
	return 5*((float)voltage/1023.0);
}
