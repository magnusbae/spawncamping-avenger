#include "MCP2515.h"
#include "SPI.h"
#include <avr/io.h>
#include <util/delay.h>

void setLoopbackOperationalMode(){
	mcp_bit_modify_instruction(MCP_CANCTRL, 0b11100000, 0b01011000);
}

void selectChip(){
	SPI_REGISTER &= ~(1<<CHIP_SELECT);
}

void deselectChip(){
	SPI_REGISTER |= (1<<CHIP_SELECT);
}

void setOperationModeNormal(){
	mcp_bit_modify_instruction(MCP_CANCTRL, 0b11100000, MODE_NORMAL);
}

void mcp_init(){
	deselectChip();
	SPI_MasterTransmit(MCP_RESET);
	//setLoopbackOperationalMode(); //debug or single node only
	setOperationModeNormal();
	mcp_write(MCP_CANINTE, MCP_RX_INT);
	mcp_bit_modify_instruction(MCP_RXB0CTRL, 0b01100000, 0xFF);
}

void mcp_clear_interrupt(){
	mcp_write(MCP_CANINTF, 0x00); //TODO needs to be fixed, can't clear all interrupts.
}

char mcp_read(char reg){
	selectChip();
	SPI_MasterTransmit(MCP_READ);
	SPI_MasterTransmit(reg);
	SPI_MasterTransmit(0xFF);
	deselectChip();
	return SPI_MasterRead();
}

void mcp_bit_modify_instruction(char reg, char mask, char data){
	selectChip();
	SPI_MasterTransmit(MCP_BITMOD);
	SPI_MasterTransmit(reg);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(data);
	deselectChip();
}

void mcp_write(char reg, char byte){
	selectChip();
	SPI_MasterTransmit(MCP_WRITE);
	SPI_MasterTransmit(reg);
	SPI_MasterTransmit(byte);
	deselectChip();
}

char mcp_read_status(){
	selectChip();
	SPI_MasterTransmit(MCP_READ_STATUS);
	SPI_MasterTransmit(0xff);
	deselectChip();
	return SPI_MasterRead();
}

int mcp_ready_to_send(int reg){
	char rtsreg = 0x00;
	switch(reg){
		case 0:
			rtsreg = TXB0CTRL;
			break;
		case 1:
			rtsreg = TXB1CTRL;
			break;
		case 2:
			rtsreg = TXB2CTRL;
			break;
		default:
			return -1;
	}
	uint8_t status = mcp_read(rtsreg);
	if((status & 0b00001000) == 8){
		return 0;
	}
	else{
		return 1;
	}
	
}

void mcp_request_to_send(int reg){
	char rtsreg = 0x00;
	switch(reg){
		case 0:
		rtsreg = MCP_RTS_TX0;
		break;
		case 1:
		rtsreg = MCP_RTS_TX1;
		break;
		case 2:
		rtsreg = MCP_RTS_TX2;
		break;
		default:
		rtsreg = MCP_RTS_ALL;
	}
	selectChip();
	SPI_MasterTransmit(rtsreg);
	deselectChip();
}

void mcp_reset(){
	selectChip();
	SPI_MasterTransmit(MCP_RESET);
	deselectChip();
}