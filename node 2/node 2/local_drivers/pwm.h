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

void pwm_move_curr_duty_cycle(float delta); //Modify current duty cycle

void pwm_update_duty(); //Updates duty cycle from current duty cycle


#endif