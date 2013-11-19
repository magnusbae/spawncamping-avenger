/*
 * AddressBus.h
 *
 * Created: 26.09.2013 14:42:03
 *  Author: magnussb
 */ 


#ifndef ADDRESSBUS_H_
#define ADDRESSBUS_H_

/**
Sets up the address bus for standard node 1 configuration.
*/
void setupAddressBus(){
	MCUCR|=(1<<SRE);
	SFIOR |= (1<<XMM2);
}


#endif /* ADDRESSBUS_H_ */