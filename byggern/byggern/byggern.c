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

volatile uint8_t JOY_CLICK = 0;


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
	
	menu_display(menu, menuLenght);

	DDRE &= ~(1<<PE0);
	cli();
	GICR |= (1<<INT2);
	sei();
	
	
	while(1){
		uint8_t menuPosition = 0;
		while(JOY_CLICK == 1){
			oled_goto_position(5, 10);
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
			
		}
		joystickPosition jp = readJoystickPosition();
		joystickDirection jd = readJoystickDirection();
		
		if(jd.direction != NEUTRAL){
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
		menu_display(menu, menuLenght);
		_delay_ms(200);
		
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


