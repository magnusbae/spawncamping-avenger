/*
 * relayDriver.c
 *
 * Created: 19.11.2013 14:23:33
 *  Author: tommp
 */ 

#include "relayDriver.h"

void initialRelaySetup(){
	RELAY_PORT_DDR |= (1<<RELAY_PIN);
	RELAY_PORT |= (1<<RELAY_PIN);
}

void triggerRelay(){
	cli();
	RELAY_PORT &= ~(1<<RELAY_PIN);
	_delay_ms(80);
	RELAY_PORT |= (1<<RELAY_PIN);
	sei();
}	