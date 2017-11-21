/*
	DRIVER FOR CONTROLLING THE MOTOR ON THE MOTOR BOARD
		- Interfaces the motor box
		- PI-controller using timer interrupts
		- Uses timer2560.h driver
		- Uses PA0 for solenoid
*/

#ifndef _MTR_CTRL_
#define _MTR_CTRL_

#include <inttypes.h>

//Regulator variables
volatile int16_t ref_vel;		//Reference value for the controller
volatile int16_t curr_vel;		//Current velocity
volatile double error_sum;		//Error sum for the discrete integral
volatile int16_t curr_u;		//Current voltage to motor
volatile int16_t curr_error;	//Current error

//Settings
#define CONTROLLER_DT 0.05	//Timestep of controller [s], should be below 260 ms at the moment and above ~10(?) ms
#define REFERENCE_DIVIDER 5.0 //Value received over CAN controller is too large for our motor, we need to scale it down

//Functions
void ctrl_init();			//Sets up the necessary registers for operation
void ctrl_regulate();		//Regulates the motor, the pad should be all the way to the left when this function is called
void ctrl_update_ref(int r); //Updates reference value	
void ctrl_apply_voltage(int v); //Applies voltage to motor
void ctrl_reset();			//Resets the reference value and error sum
void ctrl_fire_sol();		//Fires the soleinoid
void ctrl_update_KP(float p); //Updates the regulator KP value
void ctrl_update_KI(float p); //Updates the regulator KI value

#endif