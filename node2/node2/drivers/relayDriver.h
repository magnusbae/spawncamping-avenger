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
#include <avr/interrupt.h>

#define RELAY_PIN PF2
#define RELAY_PORT PORTF
#define RELAY_PORT_DDR DDRF


/**
Turns the relay on for 100ms
Should fire the actuator (solenoid) on the game board
*/
void triggerRelay();

/**
Set up the output port for relay control 
*/
void initialRelaySetup();


#endif /* INCFILE1_H_ */