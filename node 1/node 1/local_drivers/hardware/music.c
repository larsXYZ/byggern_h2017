#include "music.h"
#include <avr/io.h>
#include "f_cpu.h"
#include <util/delay.h>

//See .h file for function explanations

/*COMMUNICATION PINS*/
#define SELECT_PORT PORTB
#define SELECT_PIN PB0
#define FUNCTION_PORT PORTD
#define CTRL_MODE_PIN PD4
#define DIRECTION_PIN PD5

//Utility functions
void music_function_pin(int PIN, int v);

void music_init()
{
	//Sets up ports
	DDRB |= (1 << PB0);
	DDRD |= (1 << PD4)|(1 << PD5);
	
	//Initializes pin values
	music_function_pin(CTRL_MODE_PIN,0);
	music_function_pin(DIRECTION_PIN,0);
	
	//Completes setup
	music_start_transmission();
	_delay_ms(200);
	music_end_transmission();

}

void music_start_transmission()
{
	SELECT_PORT |= (1 << SELECT_PIN); //Sends interrupt to node 3
}

void music_end_transmission()
{
	SELECT_PORT &= ~(1 << SELECT_PIN); //Pulls it low
}

void music_start_up_sound()
{
	music_function_pin(CTRL_MODE_PIN,1);
	music_function_pin(DIRECTION_PIN,1);
	music_start_transmission();
	_delay_ms(200);
	music_end_transmission();
}

void music_function_pin(int PIN, int v)
{
	//Only works for music pins
	if (PIN != DIRECTION_PIN && PIN != CTRL_MODE_PIN) return;
	
	//Sets value on pin
	if (v == 1) FUNCTION_PORT |= (1 << PIN);
	else if (v == 0) FUNCTION_PORT &= ~(1 << PIN);
}

void music_ending_sound()
{
	music_function_pin(CTRL_MODE_PIN,1);
	music_function_pin(DIRECTION_PIN,0);
	music_start_transmission();
	_delay_ms(200);
	music_end_transmission();
}

void music_next_song()
{
	music_function_pin(CTRL_MODE_PIN,0);
	music_function_pin(DIRECTION_PIN,1);
	music_start_transmission();
	_delay_ms(200);
	music_end_transmission();
}

void music_prev_song()
{
	music_function_pin(CTRL_MODE_PIN,0);
	music_function_pin(DIRECTION_PIN,0);
	music_start_transmission();
	_delay_ms(200);
	music_end_transmission();
}
