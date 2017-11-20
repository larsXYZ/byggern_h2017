#include "oled_driver.h"
#include "ext_mem_driver.h"
#include "fonts.h"
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

//Address space
volatile uint8_t* OLED_CMD = (uint8_t*) 0x1000;
volatile uint8_t* OLED_DATA = (uint8_t*) 0x1200;
volatile uint8_t* SRAM_BUFFER = (uint8_t *) 0x1800;

//Pointers
volatile uint8_t page_index; // 0 - 7
volatile uint8_t col_index; // 0 - 123

//OLED COMMANDS
#define DISPLAY_OFF 0xae
#define SEGMENT_REMAP_MODE_0 0xa1
#define COMMON_PADS_HARDWARE 0xda
#define COMMON_OUTPUT_SCAN_DIRECTION 0xc8
#define MULTIPLEX_RATIO_MODE_63 0xa8
#define DISPLAY_DIVITE_RATIO_OSC_FREQ_MODE 0xd5
#define CONTRAST_CONTROL 0x81
#define PRE_CHARGE_PERIOD 0xd9
#define SET_MEMORY_ADDRESSING_MODE_PAGE 0x22
#define VCOM_DESELECT_LEVEL_MODE 0xdb
#define MASTER_CONFIG 0xad
#define OUT_FOLLOWS_RAM_CONTENT 0xa4
#define SET_NORMAL_DISPLAY 0xa6
#define DISPLAY_ON 0xaf 


void oled_init()
{
	//Enable external memory
	ext_mem_init();
	
	//Set row and col pointer to 0
	page_index = 0;
	col_index = 0;
	
	//Configures the oled (copied from datasheet (shamelessly))
	*OLED_CMD = DISPLAY_OFF;									//Display  off
	*OLED_CMD = SEGMENT_REMAP_MODE_0;							//Segment  remap
	*OLED_CMD = COMMON_PADS_HARDWARE;							//Common  pads  hardware:  alternative
	*OLED_CMD = 0x12;
	*OLED_CMD = COMMON_OUTPUT_SCAN_DIRECTION;					//Common output scan direction:com63~com0
	*OLED_CMD = MULTIPLEX_RATIO_MODE_63;						//Multiplex  ration  mode:63
	*OLED_CMD = 0x3f;
	*OLED_CMD = DISPLAY_DIVITE_RATIO_OSC_FREQ_MODE;				//Display divide ratio/osc. freq. mode
	*OLED_CMD = 0x80;
	*OLED_CMD = CONTRAST_CONTROL;								//Contrast  control
	*OLED_CMD = 0x50;
	*OLED_CMD = PRE_CHARGE_PERIOD;								//Set  pre-charge  period
	*OLED_CMD = 0x21;
	*OLED_CMD = SET_MEMORY_ADDRESSING_MODE_PAGE;				//Set  Memory  Addressing  Mode
	*OLED_CMD = 0x02;
	*OLED_CMD = VCOM_DESELECT_LEVEL_MODE;						//VCOM  deselect  level  mode
	*OLED_CMD = 0x30;
	*OLED_CMD = MASTER_CONFIG;									//Master  configuration
	*OLED_CMD = 0x00;
	*OLED_CMD = OUT_FOLLOWS_RAM_CONTENT;						//Out  follows  RAM  content
	*OLED_CMD = SET_NORMAL_DISPLAY;								//Set  normal  display
	*OLED_CMD = 0x40;
	*OLED_CMD = DISPLAY_ON;										//Display  on
	
	//Clears the screen
	oled_clear_screen();

	//Clear SRAM buffer
	oled_clear_SRAM();
}

void oled_go_to(uint8_t col, uint8_t page)
{
	//Sets pointers
	page_index = page;
	col_index = col; 
	
	//Set page
	*OLED_CMD = 0xB0 | (0b00000111 & page_index);
		
	//Set lower start col
	*OLED_CMD = (0b00001111 & col_index);
		
	//Set higher start col
	*OLED_CMD = 0x10 | (col_index >> 4);

}

void oled_clear_screen()
{
	//Remembers where the pointers pointed
	uint8_t save_col = col_index;
	uint8_t save_page = page_index;
	
	//Clears all pixels
	for (int page = 0; page < 8; page++)
	{
		for (int column = 0; column < 128; column++)
		{
			oled_go_to(column,page); //0-7 pages with a byte in each
			oled_page_write(0);
		}
	}
	
	//Resets pointers to previous position
	col_index = save_col;
	page_index = save_page;
}

void oled_page_write(uint8_t byte)
{
	//Set page
	*OLED_CMD = 0xB0 | (0b00000111 & page_index);

	//Set lower start col
	*OLED_CMD = (0b00001111 & col_index);

	//Set higher start col
	*OLED_CMD = 0x10 | (col_index >> 4);
	
	//Write value
	*OLED_DATA = byte;
}

int oled_char_write_big(char c)
{
	//Offsets the array index, the font doesn't have all symbols
	const int char_array_index = c - 32; 
	
	//Iterates through the font table and paints oled
	for (int i = 0; i < 8; i++)
	{
		oled_write_SRAM(pgm_read_byte(&font8[char_array_index][i]));
		oled_go_to(col_index+1,page_index);
	}
	return 0;
}


int oled_char_write_normal(char c)
{
	//Offsets the array index, the font doesn't have all symbols
	const int char_array_index = c - 32;
	
	//Iterates through the font table and paints oled
	for (int i = 0; i < 5; i++)
	{
		oled_write_SRAM(pgm_read_byte(&font5[char_array_index][i]));
		oled_go_to(col_index+1,page_index);
	}
	return 0;
}


int oled_char_write_small(char c)
{
	//Offsets the array index, the font doesn't have all symbols
	const int char_array_index = c - 32;
	
	//Iterates through the font table and paints oled
	for (int i = 0; i < 4; i++)
	{
		oled_write_SRAM(pgm_read_byte(&font4[char_array_index][i]));
		oled_go_to(col_index+1,page_index);
	}
	return 0;
}


void oled_home()
{
	oled_go_to(0,0);
}

void oled_write_SRAM(char byte)
{
	uint16_t addr = col_index + 128*page_index;//Transfers from col and row pointer to index in SRAM buffer
	if (addr < (0x0400) && addr >= 0) ext_mem_write((char *)(SRAM_BUFFER+addr),byte);
}

char oled_read_SRAM()
{
	uint16_t addr = col_index + 128*page_index;//Transfers from col and row pointer to index in SRAM buffer
	if (addr < 0x0400 && addr > 0) return ext_mem_read((char *)(SRAM_BUFFER+addr));
	else return 0; //If out of bounds, we return 
}

void oled_update_from_SRAM()
{
	//Save current row and column index
	uint8_t save_col = col_index;
	uint8_t save_page = page_index;

	//Transfer data from SRAM to OLED
	uint8_t c, p;
	for (c = 0; c < 128; c++)
	{
		for (p = 0; p < 8; p++)
		{
			oled_go_to(c,p);
			oled_page_write(oled_read_SRAM());
		}
	}

	//Recover old indexes
	col_index = save_col;
	page_index = save_page;
}

void oled_clear_SRAM()
{
	for (uint16_t i = 0; i < 0x400; i++) ext_mem_write((char *)(SRAM_BUFFER+i),0);
}

void oled_cstring_write(char *c, int size)
{	
	int i = 0;
	if (size == 0) while (c[i] != '\0'){ oled_char_write_small(c[i]); i++; }
	else if (size == 1) while (c[i] != '\0'){ oled_char_write_normal(c[i]); i++; }
	else if (size == 2) while (c[i] != '\0'){ oled_char_write_big(c[i]); i++; }	
		
}

void oled_write_pixel(int x, int y, int t)
{
	//Save current row and column index
	uint8_t save_col = col_index;
	uint8_t save_page = page_index;
	
	//Calculate position
	int page = y/8;
	int pixel_offset = y%8;
	
	int column = x;
	oled_go_to(column,page);
	
	if (t == 0)
	{
		//Turn off pixel
		char current_state = oled_read_SRAM();
		oled_write_SRAM(current_state & ~(1 << pixel_offset));
	}
	else
	{
		//Turn on pixel
		char current_state = oled_read_SRAM();
		char new_state = current_state | (1 << pixel_offset);
		oled_write_SRAM(new_state);
	}
	
	//Recover old indexes
	col_index = save_col;
	page_index = save_page;
}

void oled_write_line(int x0, int y0, int x1, int y1)
{
	//Calculates derivatives
	float dy_dx;
	float dx_dy;
	if ((x1 - x0) == 0)
	{
		dy_dx = 100000;
		dx_dy = 0;
	}
	else if (((y1 - y0) == 0))
	{
		dy_dx = 0;
		dx_dy = 100000;
	}
	else
	{
		dy_dx = ((float)(y1 - y0))/(x1 - x0);
		dx_dy = ((float)(x1 - x0))/(y1 - y0);
	}
	
	
	//Iterates over x-axis or y-axis, depending on derivative. This gives a clear line
	if (dy_dx < 1 && dy_dx > -1)
	{
		//If x1 < x0 we switch positions
		if (x1 < x0)
		{
			int xm = x1;
			int ym = y1;
			
			x1 = x0;
			y1 = y0;
			
			x0 = xm;
			y0 = ym;
		}
		
		float ypos = (float)y0;
		float xpos = (float)x0;
		oled_write_pixel(xpos,ypos,1);
		for (xpos; xpos < x1; xpos++)
		{
			ypos+=dy_dx;
			oled_write_pixel(xpos,ypos,1);
		}
		
	}
	else //Iterates over y
	{
		//If y1 < y0 we switch positions
		if (y1 < y0)
		{
			int xm = x1;
			int ym = y1;
			
			x1 = x0;
			y1 = y0;
			
			x0 = xm;
			y0 = ym;
		}
		
		float ypos = (float)y0;
		float xpos = (float)x0;
		oled_write_pixel(xpos,ypos,1);
		for (ypos; ypos < y1; ypos++)
		{
			xpos+=dx_dy;
			oled_write_pixel(xpos,ypos,1);
		}
	}
}
