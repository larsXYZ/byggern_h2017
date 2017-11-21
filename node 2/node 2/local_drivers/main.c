#include "app.h"
#include "CAN.h"
#define F_CPU 16000000
#include <util/delay.h>

int main(void)
{
	
	//Initializes drivers
	game_init();
	
	//Enters main game loop
	game_loop();

}