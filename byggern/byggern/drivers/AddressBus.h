/*
 * AddressBus.h
 *
 * Created: 26.09.2013 14:42:03
 *  Author: magnussb
 */ 


#ifndef ADDRESSBUS_H_
#define ADDRESSBUS_H_


void setupAddressBus(){
	MCUCR|=(1<<SRE);
	SFIOR |= (1<<XMM2);
}


#endif /* ADDRESSBUS_H_ */