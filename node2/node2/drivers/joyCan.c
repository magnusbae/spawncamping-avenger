#include "joyCan.h"
#include "CanMessaging.h"


int sendInputDataOverCan(){
	return 1;
}

inputMessage readReceivedInputData(canMessage messageReceived){
	volatile inputMessage im;
	im.motorPosition = messageReceived.data[1];
	im.servoPosition = messageReceived.data[2];
	im.shouldActuate = messageReceived.data[3];
	return im;
}