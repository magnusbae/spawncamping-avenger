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

#define START_GAME_COMMAND 'S'
#define STOP_GAME_COMMAND 'H'
#define CALIBRATE_GAME_COMMAND 'C'
#define SHOWBOAT_GAME_COMMAND 'P'

volatile uint8_t JOY_CLICK = 0;
volatile canMessage receivedMessage;
volatile uint8_t receivedCanMessage = 0;
volatile canMessage controlMessage;

volatile uint8_t displaychange = 1;
volatile uint8_t gameIsRunning = 0;
volatile uint8_t gameIsPaused = 0;
volatile uint8_t ackExpectedFromNode2 = 0;
volatile uint8_t ackReceivedFromNode2 = 0;

void testBallDetection();
uint8_t isBallDropped();
void sendControlMessage(char command);
void setControlMessageCommand(char command);

void handleMenuSelection(uint8_t menuPosition);
void handlePauseMenuSelection( uint8_t menuPosition );
uint8_t handleMainMenuTraversal( uint8_t joydir, joystickDirection *jd, uint8_t mainMenuLenght, menuOption * mainMenu );
uint8_t handlePauseMenuTraversal( uint8_t joydir, joystickDirection *jd, uint8_t MenuLenght, menuOption * Menu );


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	cli();
	setupUartAndSendWelcomeMessage();

	setupAddressBus();
	RamPOST();
	_delay_ms(250); //wait for oled to start
	init_oled();
	
	oled_printf("Something failed!"); //you shouldn't see this on the display

	_delay_ms(100);
	
	
	printf("Initialization complete!\r\n\r\n");

	uint8_t mainMenuLenght = 4;
	menuOption mainMenu[mainMenuLenght];
	mainMenu[0].isSelected = SELECTED;
	mainMenu[0].name = "Start new game";
	mainMenu[1].name = "Recalibrate motor";
	mainMenu[2].name = "Showboat";
	mainMenu[3].name = "Highscores";
	
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
	//cli();
	GICR |= (1<<INT2)|(1<<INT0);
	MCUCR |= (1<<ISC01); //Falling edge on INT0
	EMCUCR &= ~(1<<ISC2); //Make sure that falling edge is sat on INT2
	sei();
	
	
	SPI_MasterInit();
	mcp_init();
	
	
	controlMessage.data[0] = 'a';
	controlMessage.length = 1;
	controlMessage.extendedFrame = 0;
	controlMessage.RTR = 0;
	controlMessage.identifier = 0xAF;
	
	
	
	if(receivedCanMessage==0){
		if(CAN_send_message(controlMessage)){
			printf("\r\nCAN might have sent message. ");
		}
		else{
			printf("\r\nCAN message failed. ");
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

	uint8_t joydir = NEUTRAL;
	while(1){
		if(isBallDropped() && gameIsPaused == 0){
			gameIsPaused = 1;
			setControlMessageCommand(STOP_GAME_COMMAND);
			CAN_send_message(controlMessage);
			displaychange = 1;
		}			
				
		if(receivedCanMessage){
			receivedCanMessage = 0;
			receivedMessage = CAN_read_received_message();
			mcp_clear_interrupt();
			if(receivedMessage.data[0] == 'o' && receivedMessage.data[1] == 'k'){
				if(ackExpectedFromNode2){
					//TODO handle it
				}
			}else{
				//TODO resend-last controlMessage
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

void handleMenuSelection(uint8_t menuPosition){
	uint8_t dontSkip = 1;
	switch (menuPosition){
		case 0:
			setControlMessageCommand(START_GAME_COMMAND);
			gameIsRunning = 1;
			break;
		case 1:
			setControlMessageCommand(CALIBRATE_GAME_COMMAND);
			break;
		case 2:
			setControlMessageCommand(SHOWBOAT_GAME_COMMAND);
			break;
		case 3:
			//todo show highscore
		default:
			dontSkip = 0;
			break;		
	}
	if(dontSkip){
		CAN_send_message(controlMessage);
	}			
}

void handlePauseMenuSelection( uint8_t menuPosition ) 
{
	switch(menuPosition){
		case 1:
			setControlMessageCommand(START_GAME_COMMAND);
			CAN_send_message(controlMessage);
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

ISR(INT2_vect){
	JOY_CLICK = 1;
}

ISR(INT0_vect){
	receivedCanMessage = 1;
}

