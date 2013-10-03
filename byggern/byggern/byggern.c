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
	//setupUartAndSendWelcomeMessage();
	setupAddressBus();
	RamPOST();
	init_oled();
	
	//printf("Initialization complete!\r\n\r\n");
	
	oled_printf("fu!"); //you shouldn't see this on the display
	
	menuOption menu[4];
	menu[0].isSelected = SELECTED;
	menu[0].name = "Menyvalg 1";
	menu[1].name = "Menyvalg 3";
	menu[2].name = "Menyvalg 2";
	menu[3].name = "Menyvalg 4";
	
	menu_display(menu);

	DDRE &= ~(1<<PE0);
	cli();
	GICR |= (1<<INT2);
	sei();
	
	
	while(1){
		while(JOY_CLICK == 1){
			oled_goto_position(4, 10);
			if(JOY_CLICK == 1){
				oled_printf("Joy has been clicked");
				JOY_CLICK = 0;
				_delay_ms(1000);
			}
			oled_clear_line(4);
		}
		joystickPosition jp = readJoystickPosition();
		joystickDirection jd = readJoystickDirection();
		uint8_t leftSlider = readLeftSlider();
		uint8_t rightSlider = readRightSlider();
		printf("Joystick X: %d, Y: %d, direction: %d. Sliders, Left: %d, right: %d\r\n", jp.xPosition, jp.yPosition, jd.direction, leftSlider, rightSlider);
		_delay_ms(10);
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ISR(INT2_vect){
	JOY_CLICK = 1;
}


