#include "motor_control.h"
#include "DAC.h"
#include "drivers/f_cpu.h"
#include "timer2560.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


//PIN DEFINITIONS
#define EN PH4
#define RST_ PH6
#define O_E PH5
#define SEL PH3
#define DIR PH1
#define SOL_PIN PA0

void ctrl_init()
{
	//Enables output from pins
	DDRH = 0xFF;
	
	//Enables input from encoder
	DDRK = 0x00;
	
	//Enables DAC
	DAC_init();
	
	//Enable motor
	PORTH |= (1 << EN);
	
	//Enable !RST
	PORTH |= (1 << RST_);
	
	//Resets all values
	ctrl_reset();
	
	//Activate timer for regulating
	timer2560_init();
	
	//Activate DAC for controlling motor
	DAC_init();
	
	//Activate PA0 output
	DDRA |= (1 << SOL_PIN);
	PORTA |= (1 << SOL_PIN);
	
}

int16_t ctrl_read_encoder()
{
	
	int16_t encoder_val = 0;
	
	//Enable Encoder
	PORTH &= ~(1 << O_E);		//Clears O_E to enable encoder
	_delay_us(20);
	
	PORTH &= ~(1 << SEL);
	_delay_us(20);
	
	encoder_val |= (PINK << 8);		//Reads upper bits of encoder
	
	PORTH |= (1 << SEL);
	_delay_us(20);

	encoder_val |= (PINK << 0);		//Reads lower bits of encoder
	
	//Toggles reset
	PORTH &= ~(1 << RST_);
	_delay_us(20);
	PORTH |= (1 << RST_);
	
	//Sets O_E to disable encoder
	PORTH |= (1 << O_E);
	
	return -encoder_val;
	
}

void ctrl_regulate()
{	
	//Measure velocity
	curr_vel = ctrl_read_encoder();
	
	//Calculate error
	curr_error = CONTROLLER_DT*curr_vel - ref_vel;
	
	//Update error sum
	error_sum += CONTROLLER_DT * curr_error;
	
	//Calculate voltage
	curr_u = KP*curr_error + KI*error_sum;
	
	//Limit u
	if (curr_u > 255) curr_u = 255;
	if (curr_u < -255) curr_u = -255;
	
	//Apply 
	ctrl_apply_voltage(curr_u);
	
}

void ctrl_update_ref(int r)
{
	ref_vel = r;
}

void ctrl_reset()
{
	error_sum = 0;
	ref_vel = 0;
}

void ctrl_apply_voltage(int v)
{
	//Switch direction of motor
	if (v < 0)
	{
		PORTH |= (1 << DIR);
		v *= -1;
	}
	else
	{
		PORTH &= ~(1 << DIR);
	}
	
	DAC_write_bin(v);
}

void ctrl_fire_sol()
{
	
	PORTA &= ~(1 << SOL_PIN);
	_delay_ms(100);
	PORTA |= (1 << SOL_PIN);
	
}
