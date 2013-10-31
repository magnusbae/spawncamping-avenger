/*
 * joyCan.h
 *
 * Created: 31.10.2013 12:43:19
 *  Author: magnussb
 */ 


#ifndef JOYCAN_H_
#define JOYCAN_H_


#include "CanMessaging.h"

typedef struct
{
	char xPosition;
	char yPosition;
} joystickPosition;


int sendJoyStickPosition();
joystickPosition readReceivedJoystickPosition(canMessage messageReceived);

#endif /* JOYCAN_H_ */