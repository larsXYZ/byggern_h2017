/*
	DRIVER FOR CAN BUS
		-SUPPORTS LOOPBACK AND NORMAL MODE
		-INTERRUPT BASED
*/

#ifndef CAN
#define CAN

#include "MCP2515.h"
#include <inttypes.h>

//CAN modes of operation
#define CAN_MODE_LOOPBACK 0
#define CAN_MODE_NORMAL 1

//CAN ID types
#define ID_INPUT_UPDATE 4	//Should me message of length 5 [x_pos, y_pos, joystick_button, lbutton, rbutton]
#define ID_REGULATOR_KP 2	//Should be message of length 2 with the KP parameter, 1 -> higher bits, 2 -> lower bits
#define ID_REGULATOR_KI 3	//Should be message of length 2 with the KI parameter, 1 -> higher bits, 2 -> lower bits
#define ID_IR_SENSOR_TRIGGERED 1 //Node 1 knows that the ball is detected and the game is over


struct can_frame
{
	int id; //Identifier, lower number->higher priority
	int8_t length; //0-8
	int8_t data[8]; //Up to 8 data bytes
};

/* Driver for CAN-bus operations */

void CAN_init(char mode); //Initializes the CAN operations, mode[0-1]: 0->loopback mode, 1->normal operation

void CAN_send_frame(struct can_frame* message); //Sends a CAN message

struct can_frame CAN_receive_transmission(); //Receives transmission

int CAN_int_vector(); //Interrupt vector

int message_received_flag(); //Returns message_received flag

struct can_frame CAN_frame_init(int id, uint8_t len); //Creates an empty CAN-std-frame

void CAN_handle_message(); //Node 2 needs to do different things depending on what kind of message was received

void CAN_send_parameter(int id, float parameter);	//Node 1 needs to send regulator parameters to node 2 sometimes,
													//since CAN sends ints we use 2 bytes, and multiply the
													//wanted parameter by 1000 and send it as an int.
													//Then node 2 divides by 1000 and gets the value


#endif
