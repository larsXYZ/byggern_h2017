#include "MCP2515.h"
#include "SPI_driver.h"

void MCP_init()
{
	SPI_init();
	MCP_reset();
}

void MCP_write(char byte, char address)
{
	SPI_select_slave(1);
	SPI_send_byte(MCP_WRITE);
	SPI_send_byte(address);
	SPI_send_byte(byte);
	SPI_select_slave(0);
}

char MCP_read(char address)
{
	SPI_select_slave(1);
	SPI_send_byte(MCP_READ); //Instruction
	SPI_send_byte(address); //Address
	char received_message = SPI_receive_byte();
	SPI_select_slave(0);
	return received_message;
}

void MCP_RTS(char buffer)
{
	//Creates the instruction
	char instr = 128;
	if (buffer == 0) instr += 1;
	else if (buffer == 1) instr += 2;
	else if (buffer == 2) instr += 4;	

	//Sends instruction
	SPI_select_slave(1);
	SPI_send_byte(instr);
	SPI_select_slave(0);
}

void MCP_reset()
{
	SPI_select_slave(1);
	SPI_send_byte(MCP_RESET);
	SPI_select_slave(0);
}

char MCP_read_rx_buffer(int mode)
{
	//Creates instruction
	if (mode > 3) return 0;
	char instr = 0x80 + (mode << 1);
	
	//Executes command
	SPI_select_slave(1);
	SPI_send_byte(instr);
	char received_byte = SPI_receive_byte();
	SPI_select_slave(0);
	
	return received_byte;
}

void MCP_load_tx_buffer(int mode, char message)
{
	//Checks if settings are valid
	if (mode > 5) return;
	
	//Creates instruction
	char instr = 0x40 + mode;
	
	//Sends message
	SPI_select_slave(1);
	SPI_send_byte(instr);
	SPI_send_byte(message);
	SPI_select_slave(0);
}

char MCP_status()
{
	SPI_select_slave(1);
	SPI_send_byte(MCP_READ_STATUS);  //Instruction
	char received_data = SPI_receive_byte();
	SPI_select_slave(0);
	return received_data;
}

char MCP_rx_status()
{
	SPI_select_slave(1);
	SPI_send_byte(MCP_RX_STATUS);  //Instruction
	char received_data = SPI_receive_byte();
	SPI_select_slave(0);
	return received_data;
}

void MCP_bit_modify(char address, char mask_byte, char data_byte)
{
	SPI_select_slave(1);
	SPI_send_byte(MCP_BITMOD); //Instruction
	SPI_send_byte(address);
	SPI_send_byte(mask_byte);
	SPI_send_byte(data_byte);
	SPI_select_slave(0);
}










