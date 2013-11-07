#include "joyCan.h"
#include "multifunction.h"
#include "CanMessaging.h"
#include "uart.h"
#include <avr/interrupt.h>


int sendJoystickPosition(){
	volatile joystickPosition jp = readJoystickPosition();
	volatile canMessage m;
	
	m.extendedFrame = 0;
	m.RTR = 0;
	m.identifier = 10;
	m.length = 3;
	m.data[0] = 'j';
	m.data[1] = jp.xPosition;
	m.data[2] = jp.yPosition;
	
	return CAN_send_message(m);
}

joystickPosition readReceivedJoystickPosition(canMessage messageReceived){
	joystickPosition jp;
	jp.xPosition = messageReceived.data[1];
	jp.yPosition = messageReceived.data[2];
	return jp;
}