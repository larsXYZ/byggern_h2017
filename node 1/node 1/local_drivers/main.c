#include "App.h"

#include "music.h"
#include "f_cpu.h"
#include <util/delay.h>


int main(void)
{ /*
	app_init(); //Initializes game
	
	
	
	
		app_setup(); //Enters name, difficulty, etc
		app_run();	//Runs the game/application 
	//	if (!app_endscreen()) break; //Shows score asks if player wants to replay

	//app_goodbye_message(); //Thanks player for playing
	*/

	music_init();
	
	while (1)
	{
		music_next_song();
		_delay_ms(10000);
		music_prev_song();
		_delay_ms(10000);
	}
	
 	
	
	return 0;
}




/*
UART_init(9600); //UART, for serial
ext_mem_init();
CAN_init(CAN_MODE_NORMAL); //Initializes CAN controller
adc_init();
sei();

while (1)
{
	struct j_pos pos = adc_joy_position();
	
	struct can_frame message;
	CAN_frame_init(&message,rand()%100,3);
	message.data[0] = pos.x_p;
	message.data[1] = pos.y_p;
	message.data[2] = pos.button;
	
	//Prints joystick position
	printf("\nSENDING DATA!\nID: %d\n",message.id);
	printf("XPOS: %d\nYPOS: %d\n", message.data[0],message.data[1]);

	CAN_send_frame(&message);
	
	_delay_ms(1); */