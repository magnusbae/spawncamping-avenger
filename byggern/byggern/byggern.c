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
		//}
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

ISR(INT0_vect){
	JOY_READ = 1;
}

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; //Start address for the SRAM
	unsigned int i, werrors, rerrors;
	werrors = 0;
	rerrors = 0;
	unsigned char testvalue;
	printf("Starting SRAM test...\r\n");
	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		ext_ram[i] = testvalue;
		if (ext_ram[i] != testvalue) {
			printf("SRAM error (write phase): ext_ram[%02X] = %02X (should be %02X)\r\n", (0x1800+i), ext_ram[i], testvalue);
			werrors++;
		}
		//printf("SRAM OK (write phase): ext_ram[%d] = %02X (should be %02X). Address is %02X\r\n", i, ext_ram[i], testvalue, i);  //For verifying
	}
	for	(i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		if (ext_ram[i] != testvalue) {
			printf("SRAM error (read phase): ext_ram[%02X] = %02X (should be %02X)\r\n", (0x1800+i), ext_ram[i], testvalue);
			rerrors++;
		}
	}
	printf("SRAM test completed with %d errors in write phase and %d errors in readphase\r\n", werrors, rerrors);
}




void setupAndTestRam(){
	MCUCR|=(1<<SRE);
	SFIOR |= (1<<XMM2);
	SRAM_test();
}



