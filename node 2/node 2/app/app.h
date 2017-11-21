/*
	DRIVER WITH THE GAME LOOP FOR NODE 2
	- Listens to node 1 for control inputs
	- Listens to node 1 for parameter updates
	- Sends message to node 1 when ball is detected
*/

#ifndef _NODE2_APP_
#define _NODE2_APP_

//Functions
void game_init(); //Initializes node 2 for game operations
void game_loop(); //The main game loop for node 2
void game_ball_detected(); //Sends message to node 1 that the ball is detected with the IR-sensor, game over

#endif