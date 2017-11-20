#include "pwm.h"
#include "drivers/f_cpu.h"
#include <avr/io.h>


int pwm_init()
{

	TCCR1A |= (1 << WGM11); //Enables fast PWM mode
	TCCR1B |= (1 << WGM12) | (1 << WGM13); //Enables fast PWM mode
		
	TCCR1A |= (1 << COM1B1);//Clear output on match, set on bottom
	TCCR1B |= (5 << CS10);  //Prescaler at 1024
		
	ICR1 = F_CPU/1024/1000  *20 - 1; //Set top value
	OCR1B = F_CPU/1024/1000 *1.5 - 1; //Initializes with pulse width = 1.5ms, middle position
	DDRB |= (1 << DDB6);		//Enables output on PB6
	
	return 0;
}

void pwm_set_duty_ms(float pulse_width)
{
	//Servo cant handle the wrong pulse width
	if (pulse_width < 0.9 || pulse_width > 2.1) return;
	
	//Changes register value
	OCR1B = F_CPU/1024/1000 *pulse_width - 1;
}
