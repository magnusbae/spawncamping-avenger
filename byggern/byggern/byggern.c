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

volatile uint8_t JOY_READ = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	setupUartAndSendWelcomeMessage();
	setupAddressBus();
	RamPOST();
	init_oled();
	
	printf(" Initialization complete!\r\n\r\n");
	
	//MCUCR |= (1<<ISC01);
	//GICR |= (1<<INT0);
	//sei();
	
	
	while(1){
		joystickPosition jp = readJoystickPosition();
		joystickDirection jd = readJoystickDirection();
		uint8_t leftSlider = readLeftSlider();
		uint8_t rightSlider = readRightSlider();
		printf("Joystick X: %d, Y: %d, direction: %d. Sliders, Left: %d, right: %d\r\n", jp.xPosition, jp.yPosition, jd.direction, leftSlider, rightSlider);
		_delay_ms(1000);
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ISR(INT0_vect){
	JOY_READ = 1;
}


