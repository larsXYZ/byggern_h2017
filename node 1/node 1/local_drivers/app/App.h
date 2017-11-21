/*
			GAME APPLICATION
*/

#ifndef _APP
#define _APP

#include "oled_driver.h"
#include "menu_object.h"

//GAME VARIABLES
int CURRENT_DIFFICULTY;
char DIFFICULTY_NAME[3][5];
char PLAYER_NAME[4];
int CURRENT_SONG;
int HIGHSCORE_LIST[10];
char HIGHSCORE_LIST_NAMES[10][4];
int EXIT_APPLICATION; 


/*   Functions   */
void app_init(); //Initializes the game
void app_logo(); //Shows logo
void app_setup(); //Allows user to choose difficult, music etc
void app_run(); //Game loop
int app_endscreen(); //Shows score, allows player to restart
void app_start(); //Starts the application, -> setup -> game loop
void app_goodbye_message(); //Thanks the player for waiting


/*	Menu - actions	*/
void opt_select_name(); //selects name 
void opt_select_difficulty(); // selects level of difficulty 
void opt_select_music(); // select witch song you want to play
void view_highscore(); // view highscore
void opt_start_game(); //Starts the game loop 
void opt_exit_application();  // exits the application 

/* Utility function */
char* int_to_cstring(int t);

void the_end(); 


#endif