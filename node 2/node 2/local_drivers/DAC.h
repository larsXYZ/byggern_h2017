/*

	DRIVER FOR THE MAX520 DAC CHIP ON THE IO BOARD
		-8 BIT RESOLUTION
		-USES I2C


*/

#ifndef _DAC_
#define _DAC_

void DAC_init(); //Initializes the DAC drivers

void DAC_write_bin(unsigned char val); //Writes val to DAC via I2C, TAKES IN BINARY VALUE [0-255]

void DAC_write_v(float val); //Writes val to DAC via I2C, TAKES IN FLOAT [0.0 - 5.0]

#endif