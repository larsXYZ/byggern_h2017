/*
	DRIVER INTERFACING THE EXTERNAL SRAM CHIP
		
*/

#ifndef _EXT_MEM_
#define _EXT_MEM_


/*   Functions   */
void ext_mem_init(); //Initialize function
void ext_mem_write(char *address, char val); //Write to address
char ext_mem_read(char *address); //Read from address 0x1800 - 0x1FFF
void SRAM_test(); //Writes to all memory locations on SRAM and checks if it works
void gal_test(); //Keeps writing to SRAM to test GAL-IC

#endif
