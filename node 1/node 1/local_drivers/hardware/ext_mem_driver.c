#include "ext_mem_driver.h"
#include <stdlib.h>
#include <stdio.h>

//AVR LIBRARIES
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

void ext_mem_init()
{
	//Enables External Memory Interface
	MCUCR |= (1<<SRE);
	
	//Turns off PC7-4 for address bits, they are used as jtag anyways
	SFIOR |= (1<<XMM2);

}

void ext_mem_write(char *address, char val)
{
	*address = val; 
}

char ext_mem_read(char *address)
{
	return *(address);
}

void SRAM_test()
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors   = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	
	// rand() stores someinternal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i,retreived_value, some_value);
			write_errors++;
		}
	}
	
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n",i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	
	printf("SRAM test completed with\n%4d errors in write phase and\n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}

void gal_test()
{
	
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	
	while(1)
	{
		
		for (uint16_t i = 0; i < ext_ram_size; i++) 
		{
			uint8_t some_value = rand();
			ext_ram[i] = some_value;
		}
		
	}
}

