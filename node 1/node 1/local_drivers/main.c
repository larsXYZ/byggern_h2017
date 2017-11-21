//#include "App.h"

#include "CAN.h"
#include "music.h"
#include "f_cpu.h"
#include "ADC_driver.h"
#include <util/delay.h>
#include <avr/interrupt.h>


int main(void)
{ 

	app_init(); //Initializes game
	app_setup(); //Enters name, difficulty, etc
	app_run();	//Runs the game/application 
}
