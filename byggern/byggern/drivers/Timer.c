/*
 * Timer.c
 *
 * Created: 19.11.2013 16:02:05
 *  Author: tommp
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"

void setupTimer(){
	cli();
	TCCR1B |= (1<<CS11); //Start timer 1, 8 prescaling
	TIMSK |= (1<<TOIE1); //Enables interrupt on timer 1
	sei();
}

void destroyTimer(){
	TCCR1B &= ~(1<<CS12) | ~(1<<CS10); //Stop timer 3
	TIMSK &= ~(1<<TOIE1); //Disables interrupt on timer 3
}

void resetTimer(){
	TCNT1 = 0X0000; //Resetting counter registers (TCNT1H and TCNT1L)
	TIFR |= (1<<TOV1); //making sure the interrupt flag is cleared.
}

