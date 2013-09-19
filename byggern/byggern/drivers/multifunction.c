/*
 * multifunction.c
 *
 * Created: 19.09.2013 16:41:54
 *  Author: tommp
 */ 

#include "multifunction.h"
#include <avr/io.h>
#include <util/delay.h>

int readAdcInput(int input){
	adc[0] = input;
	_delay_ms(1);
	return adc[0];	
}

joystickPosition readJoystickPosition(){
	joystickPosition jp;
	jp.xPosition = readAdcInput(JOYSTICK_X);
	jp.yPosition = readAdcInput(JOYSTICK_Y);
	return jp;
}

int readLeftSlider(){
	return readAdcInput(TOUCH_LEFT);
}

int readRightSlider(){
	return readAdcInput(TOUCH_RIGHT);
}