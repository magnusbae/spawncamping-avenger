#include "CanMessaging.h"
#include "MCP2515.h"
#include <stdio.h>

int CAN_send_message(canMessage message){
	if(mcp_ready_to_send(0) != 1){
		return -1;
	}
	mcp_bit_modify_instruction(TXB0CTRL,0x03, 0x03); //set transmit buffer high priority
	
	if(message.extendedFrame){
		//set EIDE values
	}
	mcp_bit_modify_instruction(TXB0SIDL, 0b11100000, (message.identifier*0b100000));
	mcp_write(TXB0SIDH, message.identifier/0b1000);
	
	mcp_bit_modify_instruction(TXB0DLC, 0x0F, message.length);
	if(message.RTR){
		mcp_bit_modify_instruction(TXB0DLC, 0b01000000, 0xFF);
	}else{
		int data = TXB0D0;
		for (int i = 0; i < message.length; i++)
		{
			mcp_write(data+i, message.data[i]);	
		}
	}
	mcp_request_to_send(0);
	return 1;
}

void clearReceivedMessageBit( uint8_t address ) 
{
	mcp_bit_modify_instruction(address, 0x04, 0x00); //clear received bit
}

canMessage read_message(int reg){
	uint8_t address = 0x00;
	address = (reg == 0 ? MCP_RXB0CTRL : MCP_RXB1CTRL);
	
	canMessage received;
	
	received.length = mcp_read(address+5) & 0x0F;
	for(int i = 0; i < received.length; i++){
		received.data[i] = mcp_read(address+6+i);
	}
	clearReceivedMessageBit(address);
	return received;
}

canMessage CAN_read_received_message(){
	char status = mcp_read_status() & 0x03;
	canMessage received;
	if(status > 1){
		received = read_message(1);
	}else if(status == 1){
		received = read_message(0);
	}
	return received;
}