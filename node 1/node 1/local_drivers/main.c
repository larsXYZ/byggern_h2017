#include "App.h"
#include "CAN.h"
#include "music.h"
#include "f_cpu.h"
#include "ADC_driver.h"
#include <util/delay.h>
#include <avr/interrupt.h>


int main(void)
{ 
	printf("START\n");
	app_init(); //Initializes game
	app_setup(); //Enter name, difficulty, etc
	while (1)
	{
		if(app_main_menu()) break; //Main menu before game start
		//app_run();	//Runs the game/application
		if (app_round_review()) break;
	}
	app_goodbye_message();

}
