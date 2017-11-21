#include "CAN.h"
#ifdef __AVR_ATmega2560__
#include "../node 2/node 2/local_drivers/motor_control.h"
#include "../node 2/node 2/local_drivers/pwm.h"
#endif

#include "f_cpu.h"
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Se .h file for function explanations

volatile int message_received = 0;

void CAN_init(char mode)
{
	//Resets MCP-Values
	MCP_init();
	
	//Sets receive control register, disables rollover for RX-buffer 0
	MCP_bit_modify(MCP_RXB0CTRL,0b01100000,0xFF);

	//Interrupt when message is received
	MCP_bit_modify(MCP_CANINTE, 0x01, 0x01);
	
	//Sets mode of operation
	switch (mode)
	{
		case CAN_MODE_LOOPBACK: //Loopback mode
		{
			MCP_bit_modify(MCP_CANCTRL,0xE0,(0x40));
			break;
		}
		
		case CAN_MODE_NORMAL: //Normal operations
		{
			MCP_bit_modify(MCP_CANCTRL,0xE0,(0x00));
			break;
		}
	}
	
	//Initializing interrupt
	#ifdef __AVR_ATmega162__
	
	MCUCR |= (1<<ISC01); 
	GICR |= (1<<INT0);

	#elif __AVR_ATmega2560__
	
	EICRB |= (1<<ISC51); //Falling edge causes interrupt
	EIMSK |= (1<<INT5); //Sets PE5 as input for interrupts
	
	#endif
}

void CAN_send_frame(struct can_frame* message)
{
	
	//Sets ID
	MCP_write((message->id >> 3), MCP_TXB0SIDH); //Higher ID-bits 10 - 3
	MCP_write((message->id << 5), MCP_TXB0SIDL); //Lower ID-bits 2-0
	
	//Sets Data length
	MCP_write(message->length, MCP_TXB0DLC);
	
	//Sets Data bytes
	for (int i = 0; i < message->length; i++)
	{
		MCP_write(message->data[i], MCP_TXB0D0 + i);
	}
	
	//Sends
	MCP_RTS(0);
}

int CAN_int_vector()
{
	//Tell rest of the program we have mail
	message_received = 1;
	
	return 0;
}

//Enables interrupt vector

#ifdef __AVR_ATmega162__
ISR(INT0_vect)
{
	CAN_int_vector();
}
#elif __AVR_ATmega2560__
ISR(INT5_vect)
{
	CAN_int_vector();
}
#endif

struct can_frame CAN_receive_transmission()
{
	
	//Temporary message
	struct can_frame temp;
	for (int i = 0; i < 7; i++) temp.data[i] = 0;
	temp.id = -1;
	temp.length = 0;
	
	//Checks if we've got mail
	if (!message_received) return temp;
	else
	{
		message_received = 0;

		//Check ID
		temp.id = (MCP_read(MCP_RXB0SIDH) << 3);
		temp.id += (MCP_read(MCP_RXB0SIDL) >> 5);
	
		//Data Length
		temp.length = (MCP_read(MCP_RXB0DLC));
		
		//Data batch
		for (int i = 0; i < temp.length; i++) temp.data[i] = MCP_read(MCP_RXB0DM + i);

		MCP_bit_modify(MCP_CANINTF,0x01, 0x00);
		return temp;
	}
}

struct can_frame CAN_frame_init(int id, uint8_t len)
{
	struct can_frame t;
	t.id = id;
	t.length = len;
	for (int i = 0; i < t.length; i++) t.data[i] = 0; //Initial value = 0
	return t;
}

int message_received_flag()
{
	return message_received;
}

int CAN_handle_message()
{
	
	//Collects the message
	struct can_frame message = CAN_receive_transmission();

	switch (message.id)
	{
		case (-1):	//INVALID MESSAGE
		{
			break;
		}
		
		#ifdef __AVR_ATmega2560__  //THESE IDS ARE ONLY RELEVANT FOR NODE 2
		case (ID_INPUT_UPDATE):	//MESSAGE IS CONTROL UPDATE, [x_pos, y_pos, joystick_button, lslider]
		{
			//Sends data right to regulator
			ctrl_update_ref(message.data[0] / REFERENCE_DIVIDER);
			
			//Fires solenoid
			if (message.data[2]) ctrl_fire_sol();
			
			//Updates servo position
			pwm_set_duty_ms(1.2*message.data[3]/100.0+0.9);
			
			
			break;
		}
		case (ID_REGULATOR_KP): //MESSAGE IS UPDATE TO KP PARAMETER, //Should be message of length 2 with the KP parameter, 1 -> higher bits, 2 -> lower bits
		{
			uint16_t data = (message.data[0] << 8);
			data |= (message.data[1]);
			ctrl_update_KP(data / 1000.0);
			break;
		}
		case (ID_REGULATOR_KI): //MESSAGE IS UPDATE TO KP PARAMETER, //Should be message of length 2 with the KI parameter, 1 -> higher bits, 2 -> lower bits
		{
			uint16_t data = (message.data[0] << 8);
			data |= (message.data[1]);
			ctrl_update_KP(data / 1000.0);
			break;
		}
		#elif __AVR_ATmega162__ //THESE IDS ARE ONLY RELEVANT FOR NODE 1
		case (ID_IR_SENSOR_TRIGGERED): //Ball is detected -> game over, then it returns 1
		{
			return 1;
			break;
		}
		#endif
	}
	
	return 0;
	
}

void CAN_send_parameter(int id, float parameter)
{
	//We need to send a valid regulator parameter
	if (id != ID_REGULATOR_KI && id != ID_REGULATOR_KP) return;

	//Multiplies by 1000 and splits into lower and higher bits
	uint16_t total_par = (uint16_t)(parameter * 1000);
	uint8_t lowerbits = total_par;
	uint8_t higherbits = (total_par >> 8);

	//Constructs frame
	struct can_frame message;
	message.length = 2;
	message.data[0] = higherbits;
	message.data[1] = lowerbits;
	message.id = id;

	CAN_send_frame(&message);

}