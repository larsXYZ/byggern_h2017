#include "App.h"
#include "UART_driver.h"
#include "oled_driver.h"
#include "f_cpu.h"
#include "ADC_driver.h"
#include "menu_object.h"
#include <util\delay.h>

void opt_select_name()
{
	//Sets up screen for functions
	oled_clear_SRAM();
	oled_home();
	oled_cstring_write("Enter name: ",2);
	int i = 0;
	int ToSettings = 0; 
	
	while(ToSettings == 0)
	{
		//Read ADC
		adc_update_current_input();
		
		//Reads input
		if (joystick_down() && (int)PLAYER_NAME[i] < (int)'Z') PLAYER_NAME[i]++;
		else if (joystick_down()) PLAYER_NAME[i] = 'A';
		if (joystick_up() && (int)PLAYER_NAME[i] > (int)'A') PLAYER_NAME[i]--;
		else if (joystick_up()) PLAYER_NAME[i] = 'Z';
		if (joystick_right() && i < 3) i++;
		if (joystick_left() && i >= 0) i--;
		if ((joystick_left() && i == -1) || (joystick_right() && i == 3)) 
		{
			ToSettings = 1;
			if(i == -1) i = 0; 
			if(i == 3) i = 2; 
		}
		
		//Print current Letter
		oled_go_to(60,3);
		oled_cstring_write(PLAYER_NAME,2);
		
		oled_update_from_SRAM();
		
		
		
		if(!joystick_down() && !joystick_left() && !joystick_right() && !joystick_up())
		{
			
			_delay_ms(100);
			
			oled_go_to(60+(8*i),3);
			oled_cstring_write("_",2);
		
			oled_update_from_SRAM();
		}
		
	}
		
	}
		
void opt_select_difficulty()
{
		
		oled_clear_SRAM();
		oled_home(); 
		oled_cstring_write("Difficulty:", 2);
		
		int change = 1; 
		int completed= 0; 

		while(completed == 0) 
		{
			
			//Read ADC
			adc_update_current_input();
			
			
			//Reads input and decides min and max levels
			
			if(joystick_left()) completed = 1;
			if(joystick_right()) completed = 1;
			if (joystick_up() && CURRENT_DIFFICULTY < 2)
			{
				 CURRENT_DIFFICULTY++;
				 change = 1; 
			} 
			if (joystick_down() && CURRENT_DIFFICULTY > 0)
			{
				 CURRENT_DIFFICULTY--;
				 change = 1;
			}

			//print current level
			oled_go_to(60,3);
			oled_cstring_write(DIFFICULTY_NAME[CURRENT_DIFFICULTY],2);
			oled_update_from_SRAM();
			
			
			
			if (change!= 0)
			{
				change = 0; 
				_delay_ms(200);
			}
			
			
		}
	
}

void opt_select_music()
{
	
	
	int change = 1;
	char *song_list[15] = {"Imperial March ", "Star wars theme", "No music       "}; 
	int ToSettings= 0;
	
	oled_clear_SRAM();
	oled_home();
	oled_cstring_write("Music:", 2);

	while(ToSettings == 0)
	{
		
		//Read ADC
		adc_update_current_input();
		
		
		//Reads input and decides min and max levels
		if(joystick_left() && !change) ToSettings = 1;
		if(joystick_right() && !change) ToSettings = 1;
		if (joystick_up() && CURRENT_SONG < 2)
		{
			CURRENT_SONG++;
			change = 1;
		}
		if (joystick_down() && CURRENT_SONG > 0)
		{
			CURRENT_SONG--;
			change = 1;
		}
		
		oled_go_to(30,3);
		oled_cstring_write(song_list[CURRENT_SONG],1);
		oled_update_from_SRAM();
		
		if (change!= 0)
		{
			change = 0;
			_delay_ms(200); 
			
		}
		
		
	}
}

void view_highscore()
{
	oled_clear_SRAM();
	oled_home();
	oled_cstring_write("High score:", 2);
	int ToSettings= 0;
	
	while (ToSettings == 0)
	{
		adc_update_current_input();
		if(joystick_left())
		{
			 ToSettings = 1;
			 
		}
		
		for (int i = 0; i < 5; i += 2)
		{
			//Prints i higscore
			oled_go_to(1,2+i);
			oled_cstring_write(int_to_cstring(i+1),1);
			oled_cstring_write(". ",1);
			oled_cstring_write(HIGHSCORE_LIST_NAMES[i],1);
			oled_cstring_write(": ",1);
			oled_cstring_write(int_to_cstring(HIGHSCORE_LIST[i]),1);
			
			//Adapts next line
			if (HIGHSCORE_LIST[i] > 99) oled_cstring_write(" ",1);
			else if (HIGHSCORE_LIST[i] > 9) oled_cstring_write("  ",1);
			else oled_cstring_write("   ",1);
			
			//Prints i+1 highscore
			oled_cstring_write(int_to_cstring(i+2),1);
			oled_cstring_write(". ",1);
			oled_cstring_write(HIGHSCORE_LIST_NAMES[i+1],1);
			oled_cstring_write(": ",1);
			oled_cstring_write(int_to_cstring(HIGHSCORE_LIST[i+1]),1);
			
		}
		oled_update_from_SRAM();
		
		
	}

}

void opt_start_game()
{
	char current_score[10] = "0";  // shall increase more and more as long as the game is ongoing
	//if(atoi(current_score) > 25) atoi transform a cstring og integers to its number
	int back_to_menu = 0;
	
	
	
	oled_clear_SRAM();
	oled_home(); 
	oled_cstring_write("The game has started", 1);
	oled_update_from_SRAM();
	
	
	while (!back_to_menu) // game loop
	{
		adc_update_current_input();
		if(joystick_left()) 
		{
			oled_clear_SRAM();
			oled_home();
			oled_cstring_write("Are u sure u want to leave?", 1);
			oled_go_to(0,3); 
			oled_cstring_write("joytick left = yes",1);
			oled_go_to(0,4);
			oled_cstring_write("joytick right = no",1);
			oled_update_from_SRAM();
			
			
			while(1)
			{
				adc_update_current_input();
				if(joystick_left())
				{
					 back_to_menu = 1;
					 break;
				}
				if(joystick_right())
				{
					oled_clear_SRAM();
					break; 
					
				}
				
				
			}
			
		}
		
	    //prints current score 
		oled_home();
		oled_cstring_write("The game has started", 1);
		oled_go_to(0,3);
		oled_cstring_write("Current score: " ,1);
		oled_go_to(80,3);
		oled_cstring_write(current_score ,1);
		oled_update_from_SRAM();
		
		
		
		
	}
	
	
}

void opt_exit_application()
{
	EXIT_APPLICATION = 1; 
	
}

	
	
