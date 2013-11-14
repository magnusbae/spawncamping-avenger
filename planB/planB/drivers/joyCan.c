#include "joyCan.h"
#include "multifunction.h"
#include "CanMessaging.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint8_t sendJoyClicked_global = 0;

void resetSendJoyClickedStateToZero() 
{
	sendJoyClicked_global = 0;
}

int sendInputDataOverCan(){
	volatile joystickPosition jp = readJoystickPosition();
	volatile canMessage m;
	
	m.extendedFrame = 0;
	m.RTR = 0;
	m.identifier = 10;
	m.length = 4;
	m.data[0] = 'j';
	m.data[1] = readRightSlider();
	m.data[2] = jp.xPosition;
	m.data[3] = sendJoyClicked_global;
	
	resetSendJoyClickedStateToZero();
	return CAN_send_message(m);
}

inputMessage readReceivedInputData(canMessage messageReceived){
	volatile inputMessage im;
	im.motorPosition = messageReceived.data[1];
	im.servoPosition = messageReceived.data[2];
	im.shouldActuate = messageReceived.data[3];
	return im;
}