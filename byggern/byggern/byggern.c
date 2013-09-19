/*
 * byggern.c
 *
 * Created: 29.08.2013 12:12:10
 *  Author: tommp
 */ 
#define F_CPU 4915200UL

#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "drivers/uart.h"

void Wait(int waitTime);


volatile uint8_t JOY_READ = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	setupUartAndSendWelcomeMessage();
	setupAndTestRam();
	
	printf(" Initialization complete!\r\n\r\n");
	
	volatile char *joystick = (char *) 0x1400;
	
	
	//MCUCR |= (1<<ISC01);
	//GICR |= (1<<INT0);
	//sei();
	
	
	while(1){
		//if(JOY_READ){
			joystick[0] = 0b00000100;
			_delay_ms(1);
			printf("%d \r\n", joystick[0]);
			//JOY_READ = 0;
			_delay_ms(1000);
		//}
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ISR(INT0_vect){
	JOY_READ = 1;
}


