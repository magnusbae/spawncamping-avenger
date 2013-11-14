#include "addressBus.h"
#include <avr/io.h>


void initialAddressBusSetup(){
	XMCRA |= (1<<SRE); //Enables the External Memory Interface.The pin functions AD7:0, A15:8, ALE, WR, and RD are activated as the alternate pin functions.
	XMCRB |= (1<<XMM2) | (1<<XMM1); //Release pins PC7 to PC2 for normal use, retain PC0 and PC1 for address bus
}