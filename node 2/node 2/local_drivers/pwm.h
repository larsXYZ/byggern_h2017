/*

	DRIVER FOR CONTROLLING PWM SIGNAL TO THE SERVO
		- INTERFACES THE SERVO ON THE GAMEBOARD
		- PB6 is output pin
		- Uses timer 1
		


*/

#ifndef PWM_MODULE
#define PWM_MODULE


int pwm_init(); //Sets up the timer module

void pwm_set_duty_ms(float pulse_width); //Sets PWM signal pulse width


#endif