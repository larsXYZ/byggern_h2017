//#include "App.h"

#include "CAN.h"
#include "music.h"
#include "f_cpu.h"
#include "ADC_driver.h"
#include <util/delay.h>
#include <avr/interrupt.h>


int main(void)
{ 
	/*
	app_init(); //Initializes game
	app_setup(); //Enters name, difficulty, etc
	app_run();	//Runs the game/application 
	*/
	
	UART_init(9600); //UART, for serial
	ext_mem_init();
	CAN_init(CAN_MODE_NORMAL); //Initializes CAN controller
	adc_init();
	sei();
	int i = 0;
	
	while (1)
	{
		if (message_received_flag())
		{
			
			struct can_frame received_message = CAN_receive_transmission();
			printf("%d\n",received_message.id);
			
		}
		else
		{
			if (i++ % 5000 == 0) printf("...\n");
		}
		
	}
	
	
/*
	while (1)
	{
		struct j_pos pos = adc_joy_position();
		
		struct can_frame message = CAN_frame_init(rand()%100,3);
		message.data[0] = pos.x_p;
		message.data[1] = pos.y_p;
		message.data[2] = pos.button;
		
		//Prints joystick position
		printf("\nSENDING DATA!\nID: %d\n",message.id);
		printf("XPOS: %d\nYPOS: %d\n", message.data[0],message.data[1]);

		CAN_send_frame(&message);
		_delay_ms(1);
		
	}
	*/
	return 0;
}
