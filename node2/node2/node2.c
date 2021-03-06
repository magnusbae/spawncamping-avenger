


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

#define NODE2_REQUEST_FOR_STATUS 'a'
#define START_GAME_COMMAND 'S'
#define STOP_GAME_COMMAND 'H'
#define CALIBRATE_GAME_COMMAND 'C'
#define SHOWBOAT_GAME_COMMAND 'P'

volatile canMessage receivedMessage;
volatile canMessage message; //Message stub used for com.
volatile uint8_t receivedCanMessage = 0;
uint8_t lastMotorReference = 127;

/**
Sets all internal states according to received inputMessage
@param [in] receivedMessage The received canMessage containing inputs for gameplay
*/
void followInputs(canMessage receivedMessage);

/**
Sends an ack-message to sender with the given state
@param [in] currentState The received/assumed state.
*/
void sendCommandAck(char currentState);

/**
@name Node 2
@group node2
*/
int main(void)
{	
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
	
	 
	message.data[0] = NODE2_REQUEST_FOR_STATUS;
	message.length = 1;
	message.extendedFrame = 0;
	message.RTR = 0;
	message.identifier = 0x00;
	CAN_send_message(message);
	printf("Node 2 powered up\r\n");
	
	calibrateMotor();
	_delay_ms(1000);
	
	printf("Motor calibrated and centered. Node 2 ready to play!\r\n");

	while(1){

		if(receivedCanMessage){
			receivedCanMessage = 0;
		//	printf("Can message received with length %d \r\n", receivedMessage.length);
			//printf("Received data: %c, %i, %i, %i\r\n", receivedMessage.data[0], receivedMessage.data[1], receivedMessage.data[2], receivedMessage.data[3]);
			
			if(receivedMessage.length == 1){
				switch (receivedMessage.data[0]){
					case START_GAME_COMMAND:
						cli();
						setupTimer();
						sei();
						gameIsRunning = 1;
						sendCommandAck(START_GAME_COMMAND);
						break;
					case STOP_GAME_COMMAND:
						gameIsRunning = 0;
						cli();
						destroyTimer();
						sei();
						setDac0Output(0);
						sendCommandAck(STOP_GAME_COMMAND);
						break;
					case CALIBRATE_GAME_COMMAND:
						calibrateMotor();
						_delay_ms(3000);
						cli();
						destroyTimer();
						sei();
						break;
					case SHOWBOAT_GAME_COMMAND:
						showboat();			
						break;
					default:
						printf("Unknown command received on CAN, value: %c.\r\n", receivedMessage.data[0]);
						//Maybe handle more gracefully if we need to. This should work for now.
				}
			}		   
		}	
		if(gameIsRunning){
			followInputs(receivedMessage);
		}			
	}		
}

void followInputs(canMessage receivedMessage){
	if (receivedMessage.length == 4 && receivedMessage.data[0] == 'j'){
		volatile inputMessage receivedInput = readReceivedInputData(receivedMessage);
		//printf("Received message shouldActuate: %i", receivedInput.shouldActuate);
		set_servopos(receivedInput);
		
		setReceivedInputDataMessage(receivedInput);
		
		if(receivedInput.shouldActuate){
			triggerRelay();
			receivedInput.shouldActuate = 0;
		}
	}
	return;
}

void sendCommandAck(char currentState){
	message.data[0] = currentState;
	CAN_send_message(message);
}


/**
Interrupt routine that receives and reads in a CAN-message
*/
ISR(INT4_vect){
	receivedMessage = CAN_read_received_message();
	receivedCanMessage = 1;
	mcp_clear_interrupt();	
}	//gr�-gul, bl�-r�d, gul-svart