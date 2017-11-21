/*

	DRIVER WITH THE GAME LOOP FOR NODE 2
	- LISTENS TO NODE 1 CONTROL INPUTS
	- LISTENS TO NODE 1 PARAMETER UPDATES
	- NOTIFIES NODE 1 WHEN IR SENSOR IS CROSSED


*/

void game_init(); //Initializes node 2 for game operations

void game_loop(); //The main game loop for node 2

void game_ball_detected(); //Sends message to node 1 that the ball is detected with the IR-sensor, game over