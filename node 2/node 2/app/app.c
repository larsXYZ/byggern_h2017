#include "app.h"
#include "UART_driver.h"
#include "CAN.h"
#include "DAC.h"
#include "ADC_2560.h"
#include "motor_control.h"
#include "ir_sensor.h"
#include "pwm.h"

//Se .h file for function explanations

void game_init()
{
	//Initialises UART communication
	UART_init(9600);
	
	//CAN startup
	CAN_init(CAN_MODE_NORMAL);
	
	//DAC init
	DAC_init();
	
	//ADC init
	adc2560_init();
	
	//CTRL init
	ctrl_init();
	ctrl_update_ref(0);
	
	//Pwm init
	pwm_init();
	
	//Enables interrupts
	sei();
	
	printf("Node 2 setup completed\n");
}


void game_loop()
{
	while (1)
	{

		//Listen for messages from node 1
		CAN_handle_message();
		
		//Check if ball is detected, if it is -> notify node 1
		if (ir_detect_ball())
		{
			game_ball_detected(); //Sends message to node 1
			while(!CAN_handle_message()) 
			{
				//Waits for reset signal from node 1
				ctr_reset_integral();
			}
		}
	}
	
}

void game_ball_detected()
{
	struct can_frame message = CAN_frame_init(ID_IR_SENSOR_TRIGGERED,1);
	CAN_send_frame(&message);
}


