/*
 * ADC128.c
 *
 * Created: 06.11.2013 13:49:21
 *  Author: tommp
 */ 

#include <avr/io.h>

void initializeADC(){
	ADMUX &= ~((1<<MUX0) | (1<<MUX1) | (1<<MUX2) | (1<<MUX3) | (1<<REFS0) | (1<<REFS1)); //change these for diffrent pin and gain, REF => refrence voltage equal to AREF
	ADCSRA |= ((1<<ADEN) | (1<<ADIE) | (1<<ADPS0)); //enables the adc and interrupts TODO: set SREG!, sets prescaler to 2 
	ADCSRA &= ~(1<<ADFR); //disable free running mode
	
}

int convert(){
	ADCSRA |= (1<<ADSC); //start conversion
	while (!(ADIF)){
		//wait for conversion to complete
	}
	volatile int val_low=ADCL;
	volatile int val_high=ADCH;
	
	return val_high*0b100000000+val_low;// result is ADC=VIN*1024/VREF
}

