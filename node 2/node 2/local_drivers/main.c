
#include "drivers/f_cpu.h"
#include "drivers/UART_driver.h"
#include "drivers/CAN.h"
#include "pwm.h"
#include "ADC_2560.h"
#include "DAC.h"
#include "motor_control.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <inttypes.h>


int main(void)
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
	
	//Enables interrupts
	sei();
	
	
	printf("start\n");
	long int i = 0;
	int a = 0;
	ctrl_update_ref(0);

	while (1)
	{
		if (i++%20000 == 0)printf("running... curr_vel = %d, ref = %d, curr_u = %d, curr_error = %d, error_power = %d\n", curr_vel , ref_vel,  curr_u, curr_error,(int)(KI*error_sum));

		//Read CAN bus
		struct can_frame message = CAN_receive_transmission();	
		if (message.id != -1)
		{
			//SET JOYSTICK POSITION DATA
			int j_pos_x = message.data[0];
			if (message.data[2])
			{
				printf("FIRE SOL\n");
				ctrl_fire_sol();
			}
			
		}
	}
	
}