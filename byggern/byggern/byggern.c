/*
 * byggern.c
 *
 * Created: 29.08.2013 12:12:10
 *  Author: tommp
 */ 

#include "drivers/crystal.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "drivers/AddressBus.h"
#include "drivers/uart.h"
#include "drivers/sram.h"
#include "drivers/multifunction.h"
#include "drivers/oled.h"
#include "menu/menu.h"
#include "drivers/MCP2515.h"
#include "drivers/SPI.h"
#include "drivers/CanMessaging.h"
#include "drivers/joyCan.h"
#include "drivers/Timer.h"

#define RESEND_COMMAND_WAIT_CYCLE_THRESHOLD 3

#define NO_ACK_EXPECTED '0'
#define NODE2_REQUEST_FOR_STATUS 'a'
#define START_GAME_COMMAND 'S'
#define STOP_GAME_COMMAND 'H'
#define CALIBRATE_GAME_COMMAND 'C'
#define SHOWBOAT_GAME_COMMAND 'P'

volatile uint8_t JOY_CLICK = 0;
volatile canMessage receivedMessage;
volatile uint8_t receivedCanMessageFlag = 0;
volatile canMessage controlMessage;
volatile uint8_t joyCounter = 6;


volatile uint8_t displaychange = 1;
volatile uint8_t gameIsRunning = 0;
volatile uint8_t gameIsPaused = 0;
char ackExpectedFromNode2 = NO_ACK_EXPECTED;
uint8_t simpleNoAckReceivedCounter = 0;

/**
* Tests if balldetection is operational
*/
void testBallDetection();

/**
* Tests if ball is dropped
* @retval TRUE Ball is dropped/IR beam broken
* @retval FALSE IR beam is not broken
*/
uint8_t isBallDropped();

/**
* Sends a control message to Node2
* @param [in] command  Control command char for node 2 
*/
void sendControlMessage(char command);
/**
* Sets command character for control message
* @param [in] command  Control command char for node 2 
*/
void setControlMessageCommand(char command);

/**
* Handles joystick click if main menu was active when clicked
* @param [in] menuPosition The SELECTED menu items index
*/
void handleMenuSelection(uint8_t menuPosition);

/**
* Handles joystick click if pause menu was active when clicked
* @param [in] menuPosition The SELECTED menu items index
*/
void handlePauseMenuSelection( uint8_t menuPosition );

/**
* Handles traversal of main menu (joystick up or down)
* @param [in,out] joydir  The direction of the joystick on last cycle
* @param [in] joystickDirection The direction of the joystick when polled
* @param [in] mainMenuLength The length of the menu array
* @param [in] mainMenu 	The menuOption array
*/
uint8_t handleMainMenuTraversal( uint8_t joydir, joystickDirection *jd, uint8_t mainMenuLenght, menuOption * mainMenu );

/**
* Handles traversal of pause menu (joystick up or down).
* Unlike the main menu method, the first item in the list is not selectable. Not ideal solution to problem, but was quick and fast
* to implement.
* @param [in,out] joydir  The direction of the joystick on last cycle
* @param [in] joystickDirection The direction of the joystick when polled
* @param [in] MenuLenght The length of the menu array
* @param [in] menuOption 	The menuOption array
*/
uint8_t handlePauseMenuTraversal( uint8_t joydir, joystickDirection *jd, uint8_t MenuLenght, menuOption * Menu );


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @name Node 1
* @ingroup Node1
*/
int main(void)
{
	cli();
	setupUartAndSendWelcomeMessage();
	setupAddressBus();
	RamPOST();
	_delay_ms(250); //wait for oled to start
	init_oled();
	_delay_ms(100);
	
	
	//printf("Initialization complete!\r\n\r\n");

	uint8_t mainMenuLenght = 3;
	menuOption mainMenu[mainMenuLenght];
	mainMenu[0].isSelected = SELECTED;
	mainMenu[0].name = "Start new game";
	mainMenu[1].name = "Recalibrate motor";
	mainMenu[2].name = "Showboat";
	
	uint8_t pauseMenuLength = 3;
	menuOption pauseMenu[pauseMenuLength];
	pauseMenu[0].name = "The ball was dropped";
	pauseMenu[1].isSelected = SELECTED;
	pauseMenu[1].name = "Continue playing";
	pauseMenu[2].name = "Quit game";
	
	uint8_t playMenuLength = 1;
	menuOption playMenu[playMenuLength];
	playMenu[0].isSelected = SELECTED;
	playMenu[0].name = "Click joystick to shoot";
		
	//menu_display(menu, menuLenght, NULL);
	oled_ramclear();
	oled_ramtransfer();
	
	
	DDRE &= ~(1<<PE0);
	DDRD &= ~(1<<PD2); //PD2=INT0, PD3=INT1
	GICR |= (1<<INT2)|(1<<INT0);
	MCUCR |= (1<<ISC01); //Falling edge on INT0
	EMCUCR &= ~(1<<ISC2); //Make sure that falling edge is set on INT2
	sei();
	
	
	SPI_MasterInit();
	mcp_init();
	
	
	controlMessage.data[0] = STOP_GAME_COMMAND;
	controlMessage.length = 1;
	controlMessage.extendedFrame = 0;
	controlMessage.RTR = 0;
	controlMessage.identifier = 0xAF;
	
	
	
	if(!receivedCanMessageFlag){
		if(CAN_send_message(controlMessage)){
			//printf("\r\nCAN might have sent message. ");
		}
		else{
			//printf("\r\nCAN message failed. ");
		}			
	}	

	oled_home();
	if(isBallDropped()){
		oled_printf("Adjust IR as needed!");
		testBallDetection();
		oled_clear_line(0);
	}		
	oled_printf("IR seems to be good");
	_delay_ms(500);
	
	setupTimer();

	uint8_t joydir = NEUTRAL;
	
	while(1){
		
		if(isBallDropped() && gameIsPaused == 0){
			gameIsPaused = 1;
			ackExpectedFromNode2 = STOP_GAME_COMMAND;
			setMenuTimer();
			resetTimer();
			setControlMessageCommand(STOP_GAME_COMMAND);
			CAN_send_message(controlMessage);
			displaychange = 1;
		}			
				
		if(receivedCanMessageFlag){
			receivedCanMessageFlag = 0;
			receivedMessage = CAN_read_received_message();
			mcp_clear_interrupt();
			if(receivedMessage.length == 1){
				if(receivedMessage.data[0] == NODE2_REQUEST_FOR_STATUS){
					if(gameIsRunning && !gameIsPaused){
						ackExpectedFromNode2 = START_GAME_COMMAND;
						sendControlMessage(START_GAME_COMMAND);
					}else{
						ackExpectedFromNode2 = STOP_GAME_COMMAND;
						sendControlMessage(STOP_GAME_COMMAND);
					}
				}else if (ackExpectedFromNode2 != NO_ACK_EXPECTED){
					if(receivedMessage.data[0] != ackExpectedFromNode2){
						sendControlMessage(ackExpectedFromNode2);
					}else{
						ackExpectedFromNode2 = NO_ACK_EXPECTED;
						simpleNoAckReceivedCounter = 0;
					}
				}
			}
		}
		
		//If no ack received increment counter to treshold and resend command
		if(ackExpectedFromNode2 != NO_ACK_EXPECTED){
			simpleNoAckReceivedCounter++;
			if(simpleNoAckReceivedCounter > RESEND_COMMAND_WAIT_CYCLE_THRESHOLD){
				simpleNoAckReceivedCounter = 0;
				sendControlMessage(ackExpectedFromNode2);
			}
		}			
			
		if(gameIsRunning && !gameIsPaused){	
			sendInputDataOverCan();
		}			
		
		if(displaychange){
			if(!gameIsRunning){
				menu_display_RAMV2(mainMenu, mainMenuLenght);
			}else if(gameIsPaused){
				menu_display_RAMV2(pauseMenu, pauseMenuLength);
			}else{
				menu_display_RAMV2(playMenu, playMenuLength);
			}				
			displaychange = 0;
		}		
		
		uint8_t menuPosition = 0;
		if (JOY_CLICK == 1){
			JOY_CLICK = 0;
			
			if(gameIsRunning && !gameIsPaused){
				sendJoyClicked_global = 1;
			}else{
				displaychange = 1;
			}
			
			if(!gameIsRunning){
				for(int i = 0; i < mainMenuLenght; i++){
					if(mainMenu[i].isSelected == SELECTED){
						menuPosition = i;
					}
				}
				handleMenuSelection(menuPosition);
			}else if(gameIsPaused){
				for(int i = 0; i < pauseMenuLength; i++){
					if(pauseMenu[i].isSelected == SELECTED){
						menuPosition = i;
					}
				}
				handlePauseMenuSelection(menuPosition);
			}
			
			
		}
		
		joystickDirection jd = readJoystickDirection();
		
		if(!gameIsRunning){
			joydir = handleMainMenuTraversal(joydir, &jd, mainMenuLenght, mainMenu);
		}else if(gameIsPaused){			
			joydir = handlePauseMenuTraversal(joydir, &jd, pauseMenuLength, pauseMenu);
		}			

		_delay_ms(100);		
	}	
	
}

void setControlMessageCommand(char command){
	controlMessage.data[0] = command;
}

void sendControlMessage(char command){
	setControlMessageCommand(command);
	CAN_send_message(controlMessage);
}

void handleMenuSelection(uint8_t menuPosition){
	switch (menuPosition){
		case 0:
			ackExpectedFromNode2 = START_GAME_COMMAND;
			sendControlMessage(START_GAME_COMMAND);
			setGameTimer();
			resetTimer();
			gameIsRunning = 1;
			break;
		case 1:
			sendControlMessage(CALIBRATE_GAME_COMMAND);
			break;
		case 2:
			sendControlMessage(SHOWBOAT_GAME_COMMAND);
			break;
		case 3:
			//todo show highscore
		default:
			break;		
	}
				
}

void handlePauseMenuSelection( uint8_t menuPosition ) 
{
	switch(menuPosition){
		case 1:
			ackExpectedFromNode2 = START_GAME_COMMAND;
			setControlMessageCommand(START_GAME_COMMAND);
			CAN_send_message(controlMessage);
			setGameTimer();
			resetTimer();
			gameIsPaused = 0;
			break;
		case 2:
			gameIsPaused = 0;
			gameIsRunning = 0;
			break;
		default:
			gameIsPaused = 0;
			gameIsRunning = 0;
			break;
	}	
}


uint8_t handleMainMenuTraversal( uint8_t joydir, joystickDirection *jd, uint8_t mainMenuLenght, menuOption * mainMenu ) 
{
		if(joydir == NEUTRAL){
			if(jd->direction != NEUTRAL){
				joydir = jd->direction;
				displaychange = 1;
				if(jd->direction == UP){
					for(int i = 0; i < mainMenuLenght; i++){
						if(mainMenu[i].isSelected == SELECTED){
							mainMenu[i].isSelected = UNSELECTED;
							if(i == 0){
								mainMenu[mainMenuLenght-1].isSelected = SELECTED;
							}else{
								mainMenu[i-1].isSelected = SELECTED;
							}
							break;
						}
					}
				}else if(jd->direction == DOWN){
					for(int i = 0; i < mainMenuLenght; i++){
						if(mainMenu[i].isSelected == SELECTED){
							mainMenu[i].isSelected = UNSELECTED;
							if(i == mainMenuLenght-1){
								mainMenu[0].isSelected = SELECTED;
							}else{
								mainMenu[i+1].isSelected = SELECTED;
							}
							break;
						}
					}
				}
			}		
		}else if(jd->direction == NEUTRAL){
			joydir = NEUTRAL;
		}		return joydir;
}

uint8_t handlePauseMenuTraversal( uint8_t joydir, joystickDirection *jd, uint8_t MenuLenght, menuOption * Menu ) 
{
		if(joydir == NEUTRAL){
			if(jd->direction != NEUTRAL){
				joydir = jd->direction;
				displaychange = 1;
				if(jd->direction == UP){
					for(int i = 1; i < MenuLenght; i++){
						if(Menu[i].isSelected == SELECTED){
							Menu[i].isSelected = UNSELECTED;
							if(i == 1){
								Menu[MenuLenght-1].isSelected = SELECTED;
							}else{
								Menu[i-1].isSelected = SELECTED;
							}
							break;
						}
					}
				}else if(jd->direction == DOWN){
					for(int i = 1; i < MenuLenght; i++){
						if(Menu[i].isSelected == SELECTED){
							Menu[i].isSelected = UNSELECTED;
							if(i == MenuLenght-1){
								Menu[1].isSelected = SELECTED;
							}else{
								Menu[i+1].isSelected = SELECTED;
							}
							break;
						}
					}
				}
			}		
		}else if(jd->direction == NEUTRAL){
			joydir = NEUTRAL;
		}		return joydir;
}


uint8_t isBallDropped(){
	if(readIrDiode() < 30){
		return 1;
	}
	return 0;
}

void testBallDetection(){
	int shouldExit = 0;
	uint8_t okCount = 0;
	while(!shouldExit){
		if(!isBallDropped()){
			okCount++;
		}else{
			okCount = 0;
		}

		if(okCount >= 50){
			shouldExit = 1;
		}
	}
	return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Interrupt routine to catch timer1 overflowing.
* Allows for joystick interrupts to be caught in the main code
*/
ISR(TIMER1_OVF_vect){
		joyCounter = 1;		
}

/**
* Allows main loop to handle the click, but only if a certain time has passed.
*/
ISR(INT2_vect){
	if (joyCounter){
		JOY_CLICK = 1;
		joyCounter = 0;
	}
}

/**
* Sets active flag when receiving a CAN message
*/
ISR(INT0_vect){
	receivedCanMessageFlag = 1;
}