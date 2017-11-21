/*

	DRIVERS FOR THE PING PONG GAME BALL DETECTION FUNCTION
		- Counts score
		- Filters noise


*/

#ifndef _IR_SENSOR_
#define _IR_SENSOR_

void ir_init(); //Initialises the ir sensor software

int ir_detect_ball(); //Returns 1 if ball is detected, otherwise 0

#endif