/*
	DRIVER INTERFACING THE ADC CHIP FROM THE ATMEGA162
		- Uses multifunction card
		- PB1 and PB2 as touch buttons
		- PB3 as joystick button
		- Only sends input to node 2 when there is a change
*/

#ifndef _ADC_
#define _ADC_

#include <inttypes.h>

//Struct for returning joystick position (-100-100%)
struct j_pos
{
	int x_p;
	int y_p;
	int button;
};

//Current input struct
struct input
{
	struct j_pos joystick;
	int slider_l;
	int slider_r;
	int button_l;
	int button_r;
	
};

//Used in calibrating the joystick
volatile int x_mid_point, y_mid_point;

//Variable containing all inputs, this way we only need to use the adc once instead of each time
struct input CURRENT_INPUT;

//Functions
void adc_init(); //Mainly sets up interrupt for INT1
uint8_t adc_read(int channel); //Configures and reads adc input in single ended mode, with channel select (1-4)
void adc_test(); //Tests the Chip select for the ADC
void adc_joy_calibrate(); //Joystick calibrating function
struct j_pos adc_joy_position(); //Joystick position function, struct with 2 ints, -100 - +100 %.
int adc_slider_position_left(); //Slider position function
int adc_slider_position_right(); //Slider position function
void adc_print_inputs(); //ADC print inputs function
int adc_update_current_input(); //Returns 1 if there is any difference between the new input and the old one, with a deadzone
int compare_new_input(); //Compares new input to old, checks if there is difference
int joystick_down(); //Control functions, returns 1 if condition is true, 0 else
int joystick_up();
int joystick_left();
int joystick_right();
void send_current_input(); //Sends current input via CAN to node 2


#endif
