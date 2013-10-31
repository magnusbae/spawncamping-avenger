#include "joyCan.h"
#include "CanMessaging.h"


int sendJoyStickPosition(){
	return 1;
}

joystickPosition readReceivedJoystickPosition(canMessage messageReceived){
	volatile joystickPosition jp;
	jp.xPosition = messageReceived.data[1];
	jp.yPosition = messageReceived.data[2];
	return jp;
}