/*
 * CanMessaging.h
 *
 * Created: 10.10.2013 15:25:15
 *  Author: tommp
 */ 


#ifndef CANMESSAGING_H_
#define CANMESSAGING_H_

/**
* CAN message. Populate all fields (except unused data-fields) and send using
 CAN_send_message function
*/
typedef struct {
	/** The identifier of the CAN message (and priority, lower is better) */
	unsigned int identifier; //11 bits or 29 bits for extended frame.
	/** Set to 1 if this is an extended frame CAN message */
	unsigned int extendedFrame;
	/** Set to 1 if this is a request for the other party to send data */
	unsigned int RTR; //remote transmission request;
	/** The number of bytes in the frame (maximum 8) */
	unsigned int length;
	/** The bytes sent (maximum 8) */
	unsigned char data[8];
} canMessage;

/**
Sends a CAN message.
@retval TRUE if message was sent without problems
@retval FALSE if errors were encountered
@param [in] message the canMessage to send.
*/
int CAN_send_message(canMessage message);

/**
Reads and returns a received CAN message.
@retval canMessage The returned CAN message
*/
canMessage CAN_read_received_message();

#endif /* CANMESSAGING_H_ */