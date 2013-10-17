


#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "drivers/crystal.h"
#include "drivers/MCP2515.h"
#include "drivers/SPI.h"
#include "drivers/CanMessaging.h"


volatile canMessage receivedMessage;
volatile uint8_t receivedCanMessage = 0;

int main(void)
{
	//DDRE &= ~(1<<PE4);
	//cli();
	//EIMSK |= (1<<INT4);
	//sei();	
	
	//mcp_init();
	
	DDRF = 0xFF;
	PORTF = 0x00;
	_delay_ms(1000);
	PORTF = 0xff;
	
		canMessage message;
		message.data[0] = 'o';
		message.data[1] = 'k';
		message.length = 2;
		message.extendedFrame = 0;
		message.RTR = 0;
		message.identifier = 0x00;
		CAN_send_message(message);
	
	while(1){
		if(receivedCanMessage){
			receivedCanMessage = 0;
		}			
	}		

}

ISR(INT4_vect){
	receivedMessage = CAN_read_received_message();
	receivedCanMessage = 1;
	mcp_clear_interrupt();	
	PORTF |= (1<<PF0);
}	