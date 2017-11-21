#include "App.h"
#include "UART_driver.h"
#include "oled_driver.h"
#include "f_cpu.h"
#include "ADC_driver.h"
#include "music.h"
#include "CAN.h"

#include <util/delay.h>

/*	Menu objects   */
struct menu setup_menu;
struct option enter_name;
struct option enter_music;
struct option enter_difficulty_option;
struct option highscore; 
struct option go_to_settings;
struct option start_game;
struct option exit_application;

struct menu main_menu;

//Difficulty level:
int CURRENT_DIFFICULTY = 0;
char DIFFICULTY_NAME[3][5] = {"EASY","NORM","HARD"};

/*   Player name   */
char PLAYER_NAME[] = "AAA";

//Available songs:
int CURRENT_SONG = 0;  

//Highscore: 
int HIGHSCORE_LIST[10] = {123, 92, 62, 54, 40, 3, 0, 0, 0, 0}; 
char HIGHSCORE_LIST_NAMES[10][4] = {"LAR","PER","XXX","AKS","HEN","CHR","   ","   ","   ","   "};

int EXIT_APPLICATION = 0;




void the_end()
{
	EXIT_APPLICATION = 1;
}

void app_init()
{
	//Initialize drivers
	UART_init(9600);
	oled_init();
	adc_init();
	music_init();
	CAN_init(CAN_MODE_NORMAL);
	
	//Shows logo
	music_start_up_sound();
	app_logo();
	
	//Creating menus
	menu_constr(&setup_menu, "Settings");
	setup_menu.parent_menu = &main_menu;
	
	opt_constr(&enter_name,"Enter Name", opt_select_name);
	setup_menu.root_option = &enter_name;
	enter_name.parent = &setup_menu;
	
	opt_constr(&enter_music,"Music Selection", opt_select_music);
	enter_name.next = &enter_music;
	
	opt_constr(&enter_difficulty_option,"Difficulty", opt_select_difficulty);
	enter_music.next = &enter_difficulty_option;
	 
	
	opt_constr(&highscore, "Highscores", view_highscore);
	enter_difficulty_option.next = &highscore;
	
	
	menu_constr(&main_menu, "Main Menu");
	
	opt_constr(&start_game, "Start Game", opt_run_game); 
	main_menu.root_option = &start_game;
	
	opt_constr(&go_to_settings, "Settings", NULL);
	main_menu.root_option->next = &go_to_settings;
	go_to_settings.submenu = &setup_menu;
	
	opt_constr(&exit_application, "Exit game", opt_exit_application);
	go_to_settings.next = &exit_application; 
	
	
	//Enable interrupts
	sei();
}

void app_setup()
{
		
	opt_select_name(); 
	opt_select_difficulty();
	opt_select_music(); 
	
	//Printing menus
	while (!EXIT_APPLICATION)
	{
		adc_update_current_input();
		menu_control(&main_menu);
		oled_update_from_SRAM();
	}

}
	
void app_run()
{
	//Printing menus
	while (!EXIT_APPLICATION)
	{
		
		menu_control(&main_menu);
		oled_update_from_SRAM();
		

	}
	
	//thanks player for playing
	oled_clear_SRAM();
	oled_home(); 
	oled_cstring_write("Thank u for playing", 1);
	oled_update_from_SRAM();
	_delay_ms(1500);
	oled_clear_SRAM();
	oled_update_from_SRAM();
	
}

void app_logo()
{
	oled_go_to(1,1);
	oled_cstring_write("Made by:",2);
	
	//L
	oled_write_line(10,20,10,40);
	oled_write_line(10,40,20,40);
	
	//A
	oled_write_line(20,40,25,20);
	oled_write_line(23,30,27,30);
	oled_write_line(25,20,30,40);
	
	//R
	oled_write_line(30,40,30,20);
	oled_write_line(30,20,35,20);
	oled_write_line(35,20,35,25);
	oled_write_line(35,25,30,25);
	oled_write_line(30,25,35,40);
	
	//S
	oled_write_line(37,40,47,40);
	oled_write_line(47,40,47,30);
	oled_write_line(47,30,35,30);
	oled_write_line(37,30,37,20);
	oled_write_line(37,20,47,20);
	
	oled_go_to(52,4);
	oled_cstring_write("&",2);
	
	//P
	oled_write_line(65,40,65,20);
	oled_write_line(65,20,75,20);
	oled_write_line(75,20,75,30);
	oled_write_line(75,30,65,30);
	
	//E
	oled_write_line(77,40,77,20);
	oled_write_line(77,20,85,20);
	oled_write_line(77,30,85,30);
	oled_write_line(77,40,85,40);
	
	//R
	oled_write_line(87,40,87,20);
	oled_write_line(87,20,95,20);
	oled_write_line(95,20,95,25);
	oled_write_line(95,25,87,25);
	oled_write_line(87,25,95,40);
	
	oled_go_to(50,7);
	oled_cstring_write("TTK4155, H2017",1);

	oled_update_from_SRAM();
	_delay_ms(1000);
}

char* int_to_cstring(int t)
{
	
	char str[12];
	sprintf(str, "%d", t);
	return str;
}

void app_show_gamescreen()
{
	//prints current score
	oled_home();
	oled_clear_SRAM();
	oled_cstring_write("The game has started", 1);
	oled_go_to(0,3);
	oled_cstring_write("Current score: " ,1);
	oled_go_to(80,3);
	oled_cstring_write(int_to_cstring(CURRENT_SCORE) ,1);
	oled_update_from_SRAM();
}

void app_quit_gamescreen()
{
	oled_clear_SRAM();
	oled_home();
	oled_cstring_write("You lost", 1);
	oled_go_to(0,3);
	oled_cstring_write("joystick left = play Again",1);
	oled_go_to(0,4);
	oled_cstring_write("joystick right = Leave to main menu",1);
	oled_update_from_SRAM();
}






	


