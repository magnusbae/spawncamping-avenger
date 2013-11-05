/*
 * PWMdriver.c
 *
 * Created: 31.10.2013 15:46:19
 *  Author: magnussb
 */ 

#include "PWMdriver.h"
#include <avr/io.h>
#include "joyCan.h"

void initializePWM(){

	//Compare TCNT1(H/L) with OCR1B(H/L)
	
	OCR1BL = 1500; //Initialize to neutral position
	ICR1H = 0x4E; //Set higher top value
	ICR1L =0x1F; //Set lower top value
	TCCR1A |= ((1<<WGM11) | (1<<COM1B1)); //Set mode to fast PWM an set output sognal pin
	TCCR1B |= ((1<<WGM13) | (1<<WGM12) | (1<<CS11)); //Set Mode to fast PWM and select prescaler
	DDRB |= ((1<<DDB6)); //Set OC1B as output
}

void set_servopos(joystickPosition joypos){
	int val=joypos.xPosition*12+900;
	if (val<PULSE_MIN || val>PULSE_MAX){
		printf("ERROR: Servoposition out of range! Must be between: %d and %d", PULSE_MIN, PULSE_MAX);
	}
	else{
		OCR1BL=val;
	}
}
