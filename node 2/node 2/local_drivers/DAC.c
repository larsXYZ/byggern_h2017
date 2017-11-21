#include "DAC.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//See .h file for function explanations

#define DAC_SLAVE_ADDRESS 0x50

void DAC_init()
{
	//Initialize TWI driver from Atmel
	TWI_Master_Initialise();
	
	//Enable output on I2C pins
	DDRD |= (1 << PD1) | (1 << PD0);
	
}

void DAC_write_bin(unsigned char val)
{
	//Preventing overflow errors
	if (val > 255) val = 255;
	else if (val < 0) val = 0;
	
	unsigned char message[3];
	message[0] = DAC_SLAVE_ADDRESS;		//Set DAC as slave
	message[1] = 0;						//We use DAC0
	message[2] = val;					//Send value
	
	TWI_Start_Transceiver_With_Data(message,3);
}

void DAC_write_v(float val)
{
	//Preventing overflow errors
	if (val > 5) val = 5;
	else if (val < 0) val = 0;
	
	//Setting values
	unsigned char message[3];
	message[0] = DAC_SLAVE_ADDRESS;			//Set DAC as slave
	message[1] = 0;							//We use DAC0
	message[2] = (uint8_t)(255*val/5.0);	//Send value
		
	TWI_Start_Transceiver_With_Data(message,3);
}

