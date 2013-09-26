/*
 * srma.c
 *
 * Created: 19.09.2013 16:34:18
 *  Author: tommp
 */ 

#include "uart.h"
#include <avr/io.h>

void RamPOST(){
	SRAM_test();
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