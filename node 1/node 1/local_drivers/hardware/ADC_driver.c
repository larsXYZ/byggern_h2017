#include "ADC_driver.h"
#include "CAN.h"
#include "f_cpu.h"
#include <stdio.h>

//AVR LIBRARIES
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

//Variable keeping track if conversion is finished
volatile int finished = 0;

//Variable containing previous inputs, to limit one input registering as several
struct input PREV_INPUT;
struct input LAST_TRANSMITTED_INPUT;
#define JOYSTICK_MENU_DEADZONE 60
#define JOYSTICK_TRANSMIT_DEADZONE 7
#define JOYSTICK_ZERO_DEADZONE 7
#define SLIDER_TRANSMIT_DEADZONE 2

uint8_t adc_read(int channel)
{
	//unsigned char *p;	
	volatile char *p = (char *) 0x1400;
	
	//Selects correct address for chosen channel
	p[0] =  0x1403 + channel;
	
	//Wait for result
	while(!finished){}
	finished = 0;
	
	//Gather data
	return *p;
}

void adc_init()
{
	
	sei();
	//Sets MCU control register, for falling edge
	MCUCR |= (1<<ISC11);
	
	//Enables interrupt on pin
	GICR |= (1 << INT1);
	
	//Enable pin inputs
	DDRB &= ~((1 << PB1)|(1 << PB2)|(1 << PB3));
	
	//Calibrates joystick
	adc_joy_calibrate();
	cli();
	
	
}

ISR(INT1_vect)
{
	finished = 1;
}

void adc_joy_calibrate()
{
	//Reads midpoint & defines it as zero level
	x_mid_point = adc_read(1);
	y_mid_point = adc_read(2);
}

struct j_pos adc_joy_position()
{
	struct j_pos pos;
	
	//Working variable
	uint8_t joystick_x_analog = 0;
	uint8_t joystick_y_analog = 0;
	int joystick_x_perc = 0;
	int joystick_y_perc = 0;
	
	//Reads current position and 
	joystick_x_analog = adc_read(1);
	joystick_y_analog = adc_read(2);
	joystick_x_perc = 100*(joystick_x_analog - x_mid_point)/((float)127);
	joystick_y_perc = 100*(joystick_y_analog - y_mid_point)/((float)127);
	
	
	//Assignes value
	pos.x_p = joystick_x_perc;
	pos.y_p = joystick_y_perc;
	pos.button = !(PINB & (1 << PB3));
	
	return pos;
}

int adc_slider_position_left()
{
	return 100*adc_read(3)/((float)255);
}

int adc_slider_position_right()
{
	return 100*adc_read(4)/((float)255);
}

void adc_print_inputs()
{
	//Initializes working variables
	struct j_pos joystick_position;
	
	//Reads input ports
	if (PINB&(1<<PB1)) printf("Button left pressed\n");
	if (PINB&(1<<PB2)) printf("Button right pressed\n");
	
	//Reads ADC input
	adc_joy_position(&joystick_position);
	
	//Prints via serial port to PC
	printf("Joystick x-perc: %d\nJoystick y-perc: %d\nTouch left: %d\nTouch right: %d \n\n",joystick_position.x_p,joystick_position.y_p ,adc_slider_position_left(),adc_slider_position_right());
	
	//Delays for readability
	_delay_ms(50);
}

int adc_update_current_input()
{
	//Check if there is a difference
	int difference = compare_new_input();
	
	//CURRENT INPUT IS NOW PREVIOUS INPUT
	PREV_INPUT = CURRENT_INPUT;
	
	CURRENT_INPUT.joystick = adc_joy_position();
	
	//Checks sliders
	CURRENT_INPUT.slider_l = adc_slider_position_left();
	CURRENT_INPUT.slider_r = adc_slider_position_right();
	
	//Checks button
	CURRENT_INPUT.button_l = PINB&(1<<PB1);
	CURRENT_INPUT.button_r = PINB&(1<<PB2);
	
	//Apply deadzone
	if (CURRENT_INPUT.joystick.x_p < JOYSTICK_ZERO_DEADZONE && CURRENT_INPUT.joystick.x_p > -JOYSTICK_ZERO_DEADZONE) CURRENT_INPUT.joystick.x_p = 0;
	if (CURRENT_INPUT.joystick.y_p < JOYSTICK_ZERO_DEADZONE && CURRENT_INPUT.joystick.y_p > -JOYSTICK_ZERO_DEADZONE) CURRENT_INPUT.joystick.y_p = 0;
	
	return difference;

}

int joystick_down()
{
	return (PREV_INPUT.joystick.y_p > -JOYSTICK_MENU_DEADZONE && CURRENT_INPUT.joystick.y_p < -JOYSTICK_MENU_DEADZONE);
}

int joystick_up()
{
	return (PREV_INPUT.joystick.y_p < JOYSTICK_MENU_DEADZONE && CURRENT_INPUT.joystick.y_p > JOYSTICK_MENU_DEADZONE);
}

int joystick_left()
{
	return (PREV_INPUT.joystick.x_p > -JOYSTICK_MENU_DEADZONE && CURRENT_INPUT.joystick.x_p < -JOYSTICK_MENU_DEADZONE);
}

int joystick_right()
{
	return (PREV_INPUT.joystick.x_p < JOYSTICK_MENU_DEADZONE && CURRENT_INPUT.joystick.x_p > JOYSTICK_MENU_DEADZONE);
}

void send_current_input()
{
	struct can_frame message = CAN_frame_init(ID_INPUT_UPDATE,5);
	message.data[0] = CURRENT_INPUT.joystick.x_p;
	message.data[1] = CURRENT_INPUT.joystick.x_p;
	message.data[2] = CURRENT_INPUT.joystick.button;
	message.data[3] = CURRENT_INPUT.button_l;
	message.data[4] = CURRENT_INPUT.button_r;
	CAN_send_frame(&message);
	LAST_TRANSMITTED_INPUT = CURRENT_INPUT;
}

int compare_new_input(struct input new_input)
{
	int difference = 0;
	
	//Compare binary values
	if (PREV_INPUT.button_l != LAST_TRANSMITTED_INPUT.button_l) return 1;
	if (PREV_INPUT.button_r != LAST_TRANSMITTED_INPUT.button_r) return 10;
	if (PREV_INPUT.joystick.button != LAST_TRANSMITTED_INPUT.joystick.button) return 100;
	
	//Compare sliders
	if (PREV_INPUT.slider_l > LAST_TRANSMITTED_INPUT.slider_l + SLIDER_TRANSMIT_DEADZONE || PREV_INPUT.slider_l < LAST_TRANSMITTED_INPUT.slider_l - SLIDER_TRANSMIT_DEADZONE) return 200;
	if (PREV_INPUT.slider_r > LAST_TRANSMITTED_INPUT.slider_r + SLIDER_TRANSMIT_DEADZONE || PREV_INPUT.slider_r < LAST_TRANSMITTED_INPUT.slider_r - SLIDER_TRANSMIT_DEADZONE) return 250;
	
	//Compare joystick
	if (PREV_INPUT.joystick.x_p < LAST_TRANSMITTED_INPUT.joystick.x_p - JOYSTICK_TRANSMIT_DEADZONE || PREV_INPUT.joystick.x_p > LAST_TRANSMITTED_INPUT.joystick.x_p + JOYSTICK_TRANSMIT_DEADZONE) return 280;
	if (PREV_INPUT.joystick.y_p < LAST_TRANSMITTED_INPUT.joystick.y_p - JOYSTICK_TRANSMIT_DEADZONE || PREV_INPUT.joystick.y_p > LAST_TRANSMITTED_INPUT.joystick.y_p + JOYSTICK_TRANSMIT_DEADZONE) return 290;
	
	//Snap back to zero
	if (CURRENT_INPUT.joystick.x_p < JOYSTICK_ZERO_DEADZONE && CURRENT_INPUT.joystick.x_p > -JOYSTICK_ZERO_DEADZONE) if (LAST_TRANSMITTED_INPUT.joystick.x_p != 0) return 300;
	if (CURRENT_INPUT.joystick.y_p < JOYSTICK_ZERO_DEADZONE && CURRENT_INPUT.joystick.y_p > -JOYSTICK_ZERO_DEADZONE) if (LAST_TRANSMITTED_INPUT.joystick.y_p != 0) return 301;
	
	return difference;
}


#define JOYSTICK_TRANSMIT_DEADZONE 7
#define JOYSTICK_ZERO_DEADZONE 7
#define SLIDER_TRANSMIT_DEADZONE 2