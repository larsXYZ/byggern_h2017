/*
	SPI DRIVER
    -SUPPORTS SEVERAL SLAVES
*/

#ifndef SPI_DRIVER
#define SPI_DRIVER

#include <inttypes.h>

//Functions
void SPI_init(); //Initializes the Serial Peripheral Interface
int SPI_send_byte(uint8_t message); //Sends a byte to target
int SPI_receive_byte(); //Receives a byte from target
void SPI_select_slave(int mode); //Selects slave


#endif
