/*
 * IncFile1.h
 *
 * Created: 13.11.2013 12:16:43
 *  Author: tommp
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include <avr/io.h>
#include <avr/delay.h>

#define RELAY_PIN PF2
#define RELAY_PORT PORTF
#define RELAY_PORT_DDR DDRF

void initialRelaySetup(){
	RELAY_PORT_DDR |= (1<<RELAY_PIN);
	RELAY_PORT |= (1<<RELAY_PIN);
}

void triggerRelay(){
	RELAY_PORT &= ~(1<<RELAY_PIN);
	//printf("active, waiting");
	_delay_ms(100);
	RELAY_PORT |= (1<<RELAY_PIN);
	//printf("done, back to normal");
}


#endif /* INCFILE1_H_ */