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
struct menu main_menu;
struct menu restart_menu;

struct option enter_name;
struct option enter_music;
struct option enter_tuning_option;
struct option highscore; 
struct option go_to_settings;
struct option start_game;
struct option exit_application;
struct option restart_game;
struct option end_game;



//Tuning level:
int CURRENT_TUNING = 0;
char TUNING_NAME[3][5] = {"NORM","FAST","SLOW"};

/*   Player name   */
char PLAYER_NAME[] = "AAA";

//Available songs:
int CURRENT_SONG = 0;  

//Highscore: 
int HIGHSCORE_LIST[10] = {123, 92, 62, 54, 40, 3, 0, 0, 0, 0}; 
char HIGHSCORE_LIST_NAMES[10][4] = {"LAR","PER","XXX","AKS","HEN","CHR","   ","   ","   ","   "};

int EXIT_APPLICATION = 0;



void app_init()
{
	//Initialize drivers
	UART_init(9600);
	oled_init();
	adc_init();
	music_init();
	CAN_init(CAN_MODE_NORMAL);
	
	//Current score
	CURRENT_SCORE = 255*100;
	
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
	
	opt_constr(&enter_tuning_option,"Tuning", opt_select_tuning);
	enter_music.next = &enter_tuning_option;
	 
	
	opt_constr(&highscore, "Highscores", view_highscore);
	enter_tuning_option.next = &highscore;
	
	menu_constr(&main_menu, "Main Menu");
	
	opt_constr(&start_game, "Start Game", opt_run_game); 
	main_menu.root_option = &start_game;
	
	opt_constr(&go_to_settings, "Settings", NULL);
	main_menu.root_option->next = &go_to_settings;
	go_to_settings.submenu = &setup_menu;
	
	opt_constr(&exit_application, "Exit game", opt_exit_application);
	go_to_settings.next = &exit_application; 
	
	menu_constr(&restart_menu, "ROUND OVER");
	opt_constr(&restart_game,"Continue",opt_continue_game);
	opt_constr(&end_game, "End game",opt_end_game);
	restart_menu.root_option = &restart_game;
	restart_game.next = &end_game;
	
	//Enable interrupts
	sei();
}

void app_setup()
{
		
	opt_select_name(); 
	opt_select_tuning();
	opt_select_music(); 
}

int app_main_menu()
{
	NEXT_MENU = 0; 
	oled_clear_SRAM();
	oled_update_from_SRAM();
	menu_print(&main_menu);
	
	
	while (!NEXT_MENU)
	{
		adc_update_current_input();
		menu_control(&main_menu);
		oled_update_from_SRAM();
	}
	
	if (NEXT_MENU == 2) return 1;
	else
	{
		NEXT_MENU = 0;
		return 0;
	}
	
	
}


	
void app_run()
{
	
	//Game loop
	while (1)
	{
		
		//Update input, if the change from the last transmitted value is large enough we transmit the new values
		if(adc_update_current_input() != 0)
		{
			send_current_input();
		}
		
		//Show game screen
		app_show_gamescreen();
		
		//Handle input, checks for game over
		if (CAN_handle_message()) return;
		
		//Update score
		if (CURRENT_SCORE < MAX_SCORE) CURRENT_SCORE++;

	}
	
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
	oled_home();
	oled_clear_SRAM();
	oled_cstring_write("The game has started", 1);
	oled_go_to(0,3);
	oled_cstring_write("Current score: " ,1);
	oled_go_to(80,3);
	oled_cstring_write(int_to_cstring(CURRENT_SCORE/SCORE_DIVIDER) ,1);
	oled_update_from_SRAM();
}

void opt_exit_application()
{
	NEXT_MENU = 2;
}

void app_goodbye_message()
{
	oled_clear_SRAM();
	oled_update_from_SRAM();
	oled_go_to(20,3);
	oled_cstring_write("THANKS FOR",2);
	oled_go_to(25,4);
	oled_cstring_write("PLAYING!",2);
	oled_update_from_SRAM();
	_delay_ms(5000);
	oled_clear_SRAM();
	oled_update_from_SRAM();
}

int app_round_review()
{
	//Clears and prints menu
	oled_clear_SRAM();
	menu_print(&restart_menu);
	oled_update_from_SRAM();
	NEXT_MENU = 3;
	
	//Print score and await player choice
	while (NEXT_MENU == 3)
	{
		adc_update_current_input();
		menu_control(&restart_menu);
		oled_write_line(1,64,127,64);
		oled_go_to(5,6);
		oled_cstring_write("YOUR SCORE: ",2);
		oled_cstring_write(int_to_cstring(CURRENT_SCORE/SCORE_DIVIDER),1);
		oled_update_from_SRAM();
		
	}
	
	//Update highscore
	update_highscore();
	CURRENT_SCORE = 0;
	
	//Executes player choice
	if (NEXT_MENU) return 1;
	else
	{
		oled_clear_SRAM();
		oled_update_from_SRAM();
		menu_print(&main_menu);
		return 0;
	}
}

void opt_end_game()
{
	NEXT_MENU = 1;
}

void opt_continue_game()
{
	NEXT_MENU = 0;
}

void update_highscore()
{
	//If score is 0 you dont get on highscore
	if (CURRENT_SCORE == 0) return;
	
	//Finds your spot on the list
	int i = 0;
	while (CURRENT_SCORE/SCORE_DIVIDER < HIGHSCORE_LIST[i] && i < 10)	i++;

	//Check if you are off the list
	if (i > 9) return;
	
	//Put score on highscore list and move all other down
	for (int q = 9;q > i; q--)
	{
		HIGHSCORE_LIST[q] = HIGHSCORE_LIST[q-1];
		for (int t = 0; t < 4; t++) HIGHSCORE_LIST_NAMES[q][t] = HIGHSCORE_LIST_NAMES[q-1][t];
	}
	HIGHSCORE_LIST[i] = CURRENT_SCORE/SCORE_DIVIDER;
	for (int t = 0; t < 4; t++) HIGHSCORE_LIST_NAMES[i][t] = PLAYER_NAME[t];
}












	


