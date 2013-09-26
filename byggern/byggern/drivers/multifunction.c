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

unsigned int readAdcInput(uint8_t input){
	volatile char *adc = (char *) ADC_ADRESS;
	adc[0] = input;
	_delay_ms(1);
	return adc[0];	
}

unsigned int findJoystickDirection(){
	joystickPosition jp = readJoystickPosition();
	unsigned int direction = NEUTRAL;
	
	uint8_t xpos = jp.xPosition-NEUTRAL_POSITION;
	uint8_t ypos = jp.yPosition-NEUTRAL_POSITION;
	uint8_t abs_x = abs(xpos);
	uint8_t abs_y = abs(ypos);
	
	if (abs_x < DIRECTION_TRESHOLD && abs_y < DIRECTION_TRESHOLD)
	{
		return direction;
	}else if(abs_x > DIRECTION_TRESHOLD){
		if(xpos < 0){
			direction = LEFT;
		}else{
			direction = RIGHT;
		}
	}else{
		if(ypos > 0){
			direction = UP;
		}else{
			direction = DOWN;
		}
	}
	return direction;
}

joystickDirection readJoystickDirection(){
	joystickDirection jd;
	jd.direction = findJoystickDirection();
	return jd;
}



joystickPosition readJoystickPosition(){
	joystickPosition jp;
	jp.xPosition = readAdcInput(JOYSTICK_X);
	jp.yPosition = readAdcInput(JOYSTICK_Y);
	return jp;
}

unsigned int readLeftSlider(){
	return readAdcInput(TOUCH_LEFT);
}

unsigned int readRightSlider(){
	return readAdcInput(TOUCH_RIGHT);
}