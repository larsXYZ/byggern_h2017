/*
			GAME APPLICATION
*/

#ifndef _APP
#define _APP

#include "oled_driver.h"
#include "menu_object.h"

//CONSTANTS
#define MAX_SCORE 255*100
#define SCORE_DIVIDER 100
#define MAX_DIFFICULTY 2

//GAME VARIABLES
int CURRENT_TUNING;
char TUNING_NAME[3][5];
char PLAYER_NAME[4];
int CURRENT_SONG;
int NEXT_MENU;
int HIGHSCORE_LIST[10];
char HIGHSCORE_LIST_NAMES[10][4];
long int CURRENT_SCORE;


/*   Functions   */
void app_init(); //Initializes the game
void app_logo(); //Shows logo
void app_setup(); //Allows user to choose difficult, music etc
int app_main_menu(); //Gives access to main menu, before a game starts. Returns 1 if player quits game
void app_run(); //Game loop
int app_round_review();
void app_show_gamescreen(); //Shows the score on the oled while the game is running
int app_endscreen(); //Shows score, allows player to restart
void app_goodbye_message(); //Thanks the player for waiting


/*	Menu - actions	*/
void opt_select_name(); //selects name 
void opt_select_tuning(); // selects level of motor control tuning 
void opt_select_music(); // select witch song you want to play
void view_highscore(); // view highscore
void opt_run_game(); //Starts the game loop 
void opt_exit_application();  // exits the application
void opt_end_game(); // If player choose to not restart game, we run this function
void opt_continue_game(); //If the player choose to continue we run this function

/* Utility function */
char* int_to_cstring(int t);
void update_highscore();


#endif