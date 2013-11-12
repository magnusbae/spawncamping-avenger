


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
#include "drivers/PWMdriver.h"
#include "drivers/ADC128.h"
#include "drivers/MotorController.h"

volatile canMessage receivedMessage;
volatile uint8_t receivedCanMessage = 0;

uint8_t ifFirstMessageSendAckWithPrintf( uint8_t firstMessageIsReceived ) 
{
	if (!firstMessageIsReceived){
		printf("First message received. CAN-bus is working");
		firstMessageIsReceived = 1;
	}	
	return firstMessageIsReceived;
}

int main(void)
{	
	uint8_t firstMessageIsReceived = 0;
	uint8_t gameScore = 0;
	uint8_t gameIsRunning = 0;
	setupUartAndSendWelcomeMessage();
	
	SPI_MasterInit();
	mcp_init();
	
	initializePWM();
	initializeADC();
	initialMotorControlSetup();
	initializeEncoder();

	
	
	DDRE &= ~(1<<PE4);
	cli();
	EIMSK |= (1<<INT4);
	sei();	
	
	
	canMessage message;
	message.data[0] = 'o';
	message.data[1] = 'k';
	message.length = 2;
	message.extendedFrame = 0;
	message.RTR = 0;
	message.identifier = 0x00;
	CAN_send_message(message);
	printf("First message sent\r\n");
	
	while(1){
		int mememe = readEncoderValue();
		_delay_ms(1000);
		if(!gameIsRunning && !game_CheckBallDropped()){
			gameIsRunning = 1;
		}
		if(gameIsRunning && game_CheckBallDropped()){
			gameIsRunning = 0;
			gameScore++;
			printf("The ball was dropped, score now at -%i\r\n", gameScore);
			message.data[0] = 's';
			message.data[1] = gameScore;
			CAN_send_message(message);
			_delay_ms(1000);
		}
		
		if(receivedCanMessage){
			//firstMessageIsReceived = ifFirstMessageSendAckWithPrintf(firstMessageIsReceived);

			receivedCanMessage = 0;
			//printf("Can message received with length %d \r\n", receivedMessage.length);
			//printf("Received data: %c, %i, %i\r\n", receivedMessage.data[0], receivedMessage.data[1], receivedMessage.data[2]);
			if(receivedMessage.length == 4 && receivedMessage.data[0] == 'j'){
				volatile inputMessage receivedInput = readReceivedInputData(receivedMessage);
				//printf("Received joystickposition, x: %i y: %i \r\n", jp.xPosition, jp.yPosition);
				set_servopos(receivedInput);
				setMotorPowerFromInputData(receivedInput);
				if(receivedInput.shouldActuate){
					printf("TODO: Create relay-driver and hook up stuff\r\n");
				}
			}
		}					
	}		

}

ISR(INT4_vect){
	receivedMessage = CAN_read_received_message();
	receivedCanMessage = 1;
	mcp_clear_interrupt();	
	//PORTF |= (1<<PF0);
}	//grå-gul, blå-rød, gul-svart