/*
 * CanMessaging.h
 *
 * Created: 10.10.2013 15:25:15
 *  Author: tommp
 */ 


#ifndef CANMESSAGING_H_
#define CANMESSAGING_H_


typedef struct {
	unsigned int identifier; //11 bits or 29 bits for extended frame.
	unsigned int extendedFrame;
	unsigned int RTR; //remote transmission request;
	unsigned int length;
	unsigned char data[8];
} canMessage;

int CAN_send_message(canMessage message);
canMessage CAN_read_received_message();

#endif /* CANMESSAGING_H_ */