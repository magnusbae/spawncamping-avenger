/*
 * ADC128.c
 *
 * Doesn't work with our STK500/501! Don't use
 * Setting an internal VREF creates overvoltage on the STK, makes the motorbox beep and 
 * in general makes everybody have a very bad day. Use the free ADC-port on node 1 instead.
 * 
 */ 

#include "ADC128.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void initializeADC(){
	DDRF &= ~(1<<PF0) | ~(1<<PF1);
	ADMUX |= (1<<REFS0); // | (1<<REFS1);
	//ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3) | (1<<MUX4)); //change these for different pin and gain, REF => refrence voltage equal to AREF
	ADMUX |= 0b00010000 & 0b00011111;
	ADCSRA |= ((1<<ADEN) | (1<<ADPS0)); //enables the adc and interrupts TODO: set SREG!, sets prescaler to 2 
	ADCSRA &= ~(1<<ADFR); //disable free running mode
	
}

int adc_readvalue(){
	ADCSRA |= (1<<ADSC); //start conversion
	while (!(ADIF)){
		//wait for conversion to complete
	}
	ADCSRA |= (1<<ADIF);
	int adcvalue = ADC;
	return adcvalue;// result is ADC=VIN*1024/VREF
}

int game_CheckBallDropped(){
	if(adc_readvalue() < TRIGGERVALUE){
		return 1;
	}
	return 0;
}

