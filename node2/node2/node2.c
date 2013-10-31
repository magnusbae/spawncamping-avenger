


#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "drivers/crystal.h"
#include "drivers/MCP2515.h"
#include "drivers/SPI.h"
#include "drivers/CanMessaging.h"
#include "drivers/uart.h"

volatile canMessage receivedMessage;
volatile uint8_t receivedCanMessage = 0;

int main(void)
{	
	
	DDRE &= ~(1<<PE4);
	cli();
	EIMSK |= (1<<INT4);
	sei();	
	setupUartAndSendWelcomeMessage();
	SPI_MasterInit();
	mcp_init();
	
	write('o');
	
	//Enable some leds fosho
	//DDRB = 0xff;
	//PORTB = 0x00;
	//_delay_ms(2000);
	//PORTB = 0xfF;
	
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
			printf("Can message received");
			CAN_send_message(message);
		}			
		printf("Waiting...\r\n");
		_delay_ms(1000);
	}		

}

ISR(INT4_vect){
	receivedMessage = CAN_read_received_message();
	receivedCanMessage = 1;
	mcp_clear_interrupt();	
	PORTF |= (1<<PF0);
}	