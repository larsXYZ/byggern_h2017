/*

	DRIVER INTERFACING THE ADC CHIP FROM THE ATMEGA162
		-USES MULTIFUNCTION CARD, REMEMBER TO POWER IT
		-USES PB1 & PB2 AS TOUCHBUTTON INPUTS
		-USES PB3 AS JOYSTICK BUTTON
		


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

//Current input
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

//Mainly sets up interrupt for INT1
void adc_init();

//Configures and reads adc input in single ended mode, with channel select (1-4)
uint8_t adc_read(int channel);

//Tests the Chip select for the ADC
void adc_test();

//Joystick calibrating function
void adc_joy_calibrate();

//Joystick position function, struct with 2 ints, -100 - +100 %.
struct j_pos adc_joy_position();

//Slider position function
int adc_slider_position_left();
int adc_slider_position_right();

//ADC print inputs function
void adc_print_inputs();

//Update inputs
int adc_update_current_input(); //Returns 1 if there is any difference between the new input and the old one, with a deadzone
int compare_new_input(); //Compares new input to old, checks if there is difference


//Control functions, returns 1 if condition is true, 0 else
int joystick_down();
int joystick_up();
int joystick_left();
int joystick_right();

//Sends current input via CAN to node 2
void send_current_input();


#endif
