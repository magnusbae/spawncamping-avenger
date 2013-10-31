


#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "drivers/crystal.h"
#include "drivers/MCP2515.h"
#include "drivers/SPI.h"
#include "drivers/CanMessaging.h"
#include "drivers/uart.h"
#include "drivers/joyCan.h"

volatile canMessage receivedMessage;
volatile uint8_t receivedCanMessage = 0;

int main(void)
{	
	setupUartAndSendWelcomeMessage();
	
	SPI_MasterInit();
	mcp_init();
	
	DDRE &= ~(1<<PE4);
	cli();
	EIMSK |= (1<<INT4);
	sei();	
	
	
	//Enable some leds fosho
	//DDRB = 0xff;
	//PORTB = 0x00;
	//_delay_ms(2000);
	//PORTB = 0xf0;
	
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
			printf("Can message received with length %d \r\n", receivedMessage.length);
			printf("Received data: %c, %i, %i\r\n", receivedMessage.data[0], receivedMessage.data[1], receivedMessage.data[2]);
			if(receivedMessage.length == 3 && receivedMessage.data[0] == 'j'){
				volatile joystickPosition jp = readReceivedJoystickPosition(receivedMessage);
				printf("Received joystickposition, x: %i y: %i \r\n", jp.xPosition, jp.yPosition);
			}
			CAN_send_message(message);
		}			
		printf("Waiting...\r\n");
		_delay_ms(3000);
	}		

}

ISR(INT4_vect){
	receivedMessage = CAN_read_received_message();
	receivedCanMessage = 1;
	mcp_clear_interrupt();	
	PORTF |= (1<<PF0);
}	