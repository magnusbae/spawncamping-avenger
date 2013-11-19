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

volatile uint8_t sendJoyClicked_global;

/**
Defines the control structure needed to control gameplay on node2
*/
typedef struct
{
	/** The wanted position of the motor */
	char motorPosition;
	/** The wanted position of the servo */
	char servoPosition;
	/** Set to 1 if the relay should actuate (shoot the ball), 0 otherwise*/
	uint8_t shouldActuate;
} inputMessage;

/**
Reads inputdata from the node 1 multifunction card and transmits this data to node2 over CAN
@retval TRUE if transmission was successfull
@retvar FALSE if transmission failed
*/
int sendInputDataOverCan();

/**
Converts a CAN message of the correct format to an inputMessage
@param [in] messageReceived The received canMessage
@retval inputMessage The received and converted inputMessage
*/
inputMessage readReceivedInputData(canMessage messageReceived);

#endif /* JOYCAN_H_ */