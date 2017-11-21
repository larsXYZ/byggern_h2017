#include "timer2560.h"
#include "motor_control.h"
#include "drivers/f_cpu.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//See .h file for function explanations

int timer2560_init()
{
	TCCR3A |= (1 << WGM31) | (0 << WGM30); //ENABLE CTC MODE
	TCCR3B |= (1 << WGM32) | (0 << WGM33); //ENABLE CTC MODE
	
	TCCR3B |= (3 << CS30);  //Prescaler at 64
	
	OCR3A = (F_CPU/64) * CONTROLLER_DT; //Sets compare value according to controller DT 
	
	TIMSK3 = (1 << OCIE3A); //Enable interrupt
	
	return 0;
}

//Enables interrupt vector, regulates when timer goes off
ISR(TIMER3_COMPA_vect)
{
	ctrl_regulate();
}

