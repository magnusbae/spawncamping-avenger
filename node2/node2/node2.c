


#include "drivers/crystal.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "drivers/MCP2515.h"
#include "drivers/SPI.h"
#include "drivers/CanMessaging.h"
#include "drivers/uart.h"
#include "drivers/joyCan.h"
#include "drivers/PWMdriver.h"
#include "drivers/MotorController.h"
#include "drivers/relayDriver.h"

volatile canMessage receivedMessage;
volatile uint8_t receivedCanMessage = 0;
uint8_t lastMotorReference = 127;


void calibrateMotor();
void followInputs(canMessage receivedMessage);

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
	initialMotorControlSetup();
	initializeEncoder();

	initialRelaySetup();
	
	
	DDRE &= ~(1<<PE4);
	cli();
	EIMSK |= (1<<INT4); //Interrupt on received CAN-message.
	sei();	
	
	
	canMessage message;
	message.data[0] = 'o';
	message.data[1] = 'k';
	message.length = 2;
	message.extendedFrame = 0;
	message.RTR = 0;
	message.identifier = 0x00;
	CAN_send_message(message);
	printf("Node 2 powered up\r\n");
	
	calibrateMotor();
	
	CAN_send_message(message);
	printf("Motor calibrated and centered. Node 2 ready to play!");

	while(1){
		//checkEncoder();
		int mememe = readEncoderValue();
		printf("Encoder value: %i\r\n", mememe);
		
		if(receivedCanMessage){
			receivedCanMessage = 0;
			//printf("Can message received with length %d \r\n", receivedMessage.length);
			//printf("Received data: %c, %i, %i\r\n", receivedMessage.data[0], receivedMessage.data[1], receivedMessage.data[2]);
			
			if(receivedMessage.length == 1){
				switch (receivedMessage.data[0]){
					case 'S':
						gameIsRunning = 1;
						break;
					case 'H':
						gameIsRunning = 0;
						break;
					case 'C':
						calibrateMotor();
						break;
					case 'P':
						//TODO Make some noise or something.
						
						break;
					default:
						printf("Unknown command received on CAN, value: %c.\r\n", receivedMessage.data[0]);
						//Maybe handle more gracefully if we need to. This should work for now.
				}
				CAN_send_message(message);
			}
			if(gameIsRunning){
				followInputs(receivedMessage);
			}
		   
		}				
	}		
}

void followInputs(canMessage receivedMessage){
	if (receivedMessage.length == 4 && receivedMessage.data[0] == 'j'){
		volatile inputMessage receivedInput = readReceivedInputData(receivedMessage);
		//printf("Received message shouldActuate: %i", receivedInput.shouldActuate);
		set_servopos(receivedInput);
		if (receivedInput.motorPosition>lastMotorReference+15 || receivedInput.motorPosition<lastMotorReference-15){
			lastMotorReference=receivedInput.motorPosition;
		}
		
		regulator(receivedInput);
		
		if(receivedInput.shouldActuate){
			triggerRelay();
		}
	}
	return;
}


ISR(INT4_vect){
	receivedMessage = CAN_read_received_message();
	receivedCanMessage = 1;
	mcp_clear_interrupt();	
	//PORTF |= (1<<PF0);
}	//grå-gul, blå-rød, gul-svart