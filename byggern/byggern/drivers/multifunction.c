/*
 * multifunction.c
 *
 * Created: 19.09.2013 16:41:54
 *  Author: tommp
 */ 

#include "crystal.h"
#include "multifunction.h"
#include <avr/io.h>
#include <util/delay.h>

joystickPosition getJoystickPosition();

uint8_t readAdcInput(uint8_t input){
	volatile char *adc = (char *) ADC_ADRESS;
	adc[0] = input;
	_delay_ms(1);
	return adc[0];	
}

uint8_t calculateJoystickDirection(joystickPosition jp){
	uint8_t direction = NEUTRAL;
	
	uint8_t xpos = jp.xPosition;
	uint8_t ypos = jp.yPosition;

	
	if (((xpos < DIRECTION_TRESHOLD_POSITIVE) && (xpos > DIRECTION_TRESHOLD_NEGATIVE))  && ((ypos < DIRECTION_TRESHOLD_POSITIVE) && (ypos > DIRECTION_TRESHOLD_NEGATIVE)))
	{
		return direction;
	}else if((xpos > DIRECTION_TRESHOLD_POSITIVE) || (xpos < DIRECTION_TRESHOLD_NEGATIVE)){
		if(xpos < DIRECTION_TRESHOLD_NEGATIVE){
			direction = LEFT;
		}else{
			direction = RIGHT;
		}
	}else{
		if(ypos > DIRECTION_TRESHOLD_POSITIVE){
			direction = UP;
		}else{
			direction = DOWN;
		}
	}
	return direction;
}

uint8_t findJoystickDirection(){
	joystickPosition jp = getJoystickPosition();
	return calculateJoystickDirection(jp);
}

joystickDirection readJoystickDirection(){
	joystickDirection jd;
	jd.direction = findJoystickDirection();
	return jd;
}


joystickPosition getJoystickPosition(){
	joystickPosition jp;
	jp.xPosition = readAdcInput(JOYSTICK_X);
	jp.yPosition = readAdcInput(JOYSTICK_Y);
	return jp;
}

joystickPosition readJoystickPosition(){
	joystickPosition jp = getJoystickPosition();
	jp.xPosition = ((255*100)-(jp.xPosition*100))/255;
	jp.yPosition = ((255*100)-(jp.yPosition*100))/255;
	if (jp.xPosition < 54 && jp.xPosition > 46){
		jp.xPosition = 50;
	}
	if (jp.yPosition < 54 && jp.yPosition > 46){
		jp.yPosition = 50;
	}
	return jp;
}

uint8_t readIrDiode(){
	return readAdcInput(IR_DIODE);
}

uint8_t readRightSlider(){
	return readAdcInput(TOUCH_RIGHT);
}