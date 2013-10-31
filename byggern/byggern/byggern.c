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

volatile uint8_t JOY_CLICK = 0;
volatile canMessage receivedMessage;
volatile uint8_t receivedCanMessage = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	setupUartAndSendWelcomeMessage();
	setupAddressBus();
	RamPOST();
	init_oled();
	
	printf("Initialization complete!\r\n\r\n");
	
	oled_printf("Something failed!"); //you shouldn't see this on the display
	
	uint8_t menuLenght = 4;
	menuOption menu[menuLenght];
	menu[0].isSelected = SELECTED;
	menu[0].name = "Menyvalg 1";
	menu[1].name = "Menyvalg 2";
	menu[2].name = "Menyvalg 3";
	menu[3].name = "Menyvalg 4";
	
	
	//menu_display(menu, menuLenght, NULL);
	oled_ramclear();
	oled_ramtransfer();
	
	
	DDRE &= ~(1<<PE0);
	DDRD &= ~(1<<PD2); //PD2=INT0, PD3=INT1
	cli();
	GICR |= (1<<INT2)|(1<<INT0);
	sei();
	
	
	SPI_MasterInit();
	mcp_init();
	printf("%02X", mcp_read(MCP_CANSTAT));
	
	canMessage message;
	message.data[0] = 0xF1;
	message.length = 1;
	message.extendedFrame = 0;
	message.RTR = 0;
	message.identifier = 0xAF;
	//if(CAN_send_message(message)){
		//printf("\r\nCAN might have sent message. ");
	//}		
	
	uint8_t displaychange = 1;
	uint8_t joydir = NEUTRAL;
	while(1){
			if(receivedCanMessage){
				receivedCanMessage = 0;
				printf("Received CAN message with data length %d", receivedMessage.length);
				for (int i = 0; i < receivedMessage.length; i++){
					printf(" %c", receivedMessage.data[i]);
				}					
				
				message.data[0] = 0x12;
				message.length = 8;
				if(CAN_send_message(message)){
					printf("\r\nCAN might have sent message. ");
				}
			}
		if(displaychange){
			menu_display_RAMV2(menu, menuLenght);
			displaychange = 0;
		}		
		
		uint8_t menuPosition = 0;
		while(JOY_CLICK == 1){
			oled_goto_position(5, 0);
			if(JOY_CLICK == 1){
				for(int i = 0; i < menuLenght; i++){
					if(menu[i].isSelected == SELECTED){
						menuPosition = i;
					}
				}
				oled_printf("Joy has been clicked, ");
				oled_printf(menu[menuPosition].name);
				
				JOY_CLICK = 0;
				_delay_ms(1000);
			}
			oled_clear_line(5);
			oled_clear_line(6);
			
		}
		joystickPosition jp = readJoystickPosition();
		joystickDirection jd = readJoystickDirection();
		
		if(joydir == NEUTRAL){
			if(jd.direction != NEUTRAL){
				joydir = jd.direction;
				displaychange = 1;
				if(jd.direction == UP){
					for(int i = 0; i < menuLenght; i++){
						if(menu[i].isSelected == SELECTED){
							menu[i].isSelected = UNSELECTED;
							if(i == 0){
								menu[menuLenght-1].isSelected = SELECTED;
							}else{
								menu[i-1].isSelected = SELECTED;
							}
							break;
						}
					}
				}else if(jd.direction == DOWN){
					for(int i = 0; i < menuLenght; i++){
						if(menu[i].isSelected == SELECTED){
							menu[i].isSelected = UNSELECTED;
							if(i == menuLenght-1){
								menu[0].isSelected = SELECTED;
							}else{
								menu[i+1].isSelected = SELECTED;
							}
							break;
						}
					}
				}
			}		
		}else if(jd.direction == NEUTRAL){
			joydir = NEUTRAL;
		}			
		uint8_t leftSlider = readLeftSlider();
		uint8_t rightSlider = readRightSlider();
		//printf("Joystick X: %d, Y: %d, direction: %d. Sliders, Left: %d, right: %d\r\n", jp.xPosition, jp.yPosition, jd.direction, leftSlider, rightSlider);
		_delay_ms(10);
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ISR(INT2_vect){
	JOY_CLICK = 1;
}

ISR(INT0_vect){
	receivedMessage = CAN_read_received_message();
	receivedCanMessage = 1;
	mcp_clear_interrupt();
}
