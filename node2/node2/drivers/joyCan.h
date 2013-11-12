/*
 * joyCan.h
 *
 * Created: 31.10.2013 12:43:19
 *  Author: magnussb
 */ 


#ifndef JOYCAN_H_
#define JOYCAN_H_


#include "CanMessaging.h"
#include <avr/io.h>

typedef struct
{
	char motorPosition;
	char servoPosition;
	uint8_t shouldActuate;
} inputMessage;


int sendInputDataOverCan();
inputMessage readReceivedInputData(canMessage messageReceived);

#endif /* JOYCAN_H_ */