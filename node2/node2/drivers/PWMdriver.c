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

	TCCR1A |= ((1<<WGM11) | (1<<COM1B1)); //Set mode to fast PWM an set output signal pin
	TCCR1A &= ~((1<<COM1B0) | (1<<WGM10));
	TCCR1B |= ((1<<WGM13) | (1<<WGM12) | (1<<CS11)); //Set Mode to fast PWM and select prescaler(8)
	TCCR1B &= ~((1<<CS12) | (1<<CS10));

	OCR1BH = 1524/0b100000000;
	OCR1BL = 1524; //Initialize to neutral position
	ICR1H = 0x51; //Set higher top value
	ICR1L = 0x40; //Set lower top value
	DDRB |= ((1<<PB6)); //Set OC1B as output
}

void set_servopos(inputMessage joypos){
	float multiplier = (PULSE_MAX-PULSE_MIN)/100.00;
	int val = 1524;
	if (joypos.motorPosition>53 || joypos.motorPosition<47){
		val=(joypos.motorPosition*multiplier)+PULSE_MIN;
	}		
	if (val<PULSE_MIN || val>PULSE_MAX){
		printf("ERROR: Servoposition out of range! Must be between: %d and %d", PULSE_MIN, PULSE_MAX);
	}
	else{
		OCR1B = val;
	}
}
