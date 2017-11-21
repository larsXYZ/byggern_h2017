/*
      DRIVER FOR THE OLED SCREEN ON THE MULTIFUNCTION BOARD
		- Uses SRAM as buffer, this enables pixel level interaction with the OLED
*/

#ifndef OLED_DRIVER
#define OLED_DRIVER

#include <inttypes.h>
#include <stdio.h>

/*   FUNCTIONS   */
void oled_init(); //Initializes the oled
void oled_go_to(uint8_t col, uint8_t page); //Moves pointer to a location on screen
void oled_page_write(uint8_t byte); //Write byte at row and column
int oled_char_write_big(char c); //Write a single character at pointer, with big font
int oled_char_write_normal(char c); //Write a single character at pointer, with normal font
int oled_char_write_small(char c); //Write a single character at pointer, with small font
void oled_cstring_write(char *c, int size); //Custom function for writing c-strings, size [0 - 2], 2->Big 1->Normal 0->Small
void oled_clear_screen(); //Clear screen
void oled_home(); //Moves cursor to top of screen
void oled_write_SRAM(char byte); //Writes byte to location in SRAM
char oled_read_SRAM(); //Reads whats on the SRAM buffer at current location
void oled_update_from_SRAM();	//Updates entire screen from SRAM
void oled_write_pixel(int x, int y, int t); //Sets pixel off or on
void oled_clear_SRAM(); //Clears SRAM buffer
void oled_write_line(int x0, int y0, int x1, int y1); //Writes line from p0 to p1
extern FILE oled_stdout; //Provides food for fprintf

#endif
