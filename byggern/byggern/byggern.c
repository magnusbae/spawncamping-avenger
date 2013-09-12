/*
 * byggern.c
 *
 * Created: 29.08.2013 12:12:10
 *  Author: tommp
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
void write(char val);
char read_();
void set_initial(uint8_t baud_rate_value);

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800;
	//Start address for the SRAM
	unsigned int i, werrors, rerrors;
	werrors = 0;
	rerrors = 0;
	unsigned char testvalue;
	printf("Starting SRAM test...\r\n");
	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		ext_ram[i] = testvalue;
		if (ext_ram[i] != testvalue) {
			printf("SRAM error (write phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			werrors++;
		}
	}
	for
	(i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		if (ext_ram[i] != testvalue) {
			printf("SRAM error (read phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			rerrors++;
		}
	}
	printf("SRAM test completed with %d errors in write phase and %d errors in readphase\r\n", werrors, rerrors);
}

void Wait(int waitTime)
	{
		for(uint8_t i=0;i<waitTime;i++)
		{
			_delay_loop_2(0);
		}
	}
	
int main(void)
{
	set_initial(31);
	fdevopen(write, read_);
	print_welcome();
	printf("UART Initialized\r\n");
	
	MCUCR|=(1<<SRE);

	SRAM_test();
	printf("Test complete!\r\n\r\n");
    while(1){
	}

		
		
		
			        
}

void print_welcome(){
	printf("__________                                          \r\n");
	printf("\\______   \\___.__. ____   ____   ___________  ____  \r\n");
	printf(" |    |  _<   |  |/ ___\\ / ___\\_/ __ \\_  __ \\/    \\ \r\n");
	printf(" |    |   \\\\___  / /_/  > /_/  >  ___/|  | \\/   |  \\ \r\n");
	printf(" |______  // ____\\___  /\\___  / \\___  >__|  |___|  /\r\n");
	printf("        \\/ \\/   /_____//_____/      \\/           \\/ \r\n");
}

void set_initial(uint8_t baud_rate_value){
	
	
	UBRR0H=(8>>baud_rate_value);
	UBRR0L=baud_rate_value;
	UCSR0C|=(1<<URSEL0)|(1<<UCSZ01)|(1<<UCSZ00);
	UCSR0B|=(1<<RXEN0)|(1<<TXEN0);
	UCSR0B|=(1<<RXCIE0)|(1<<TXCIE0);
}

char read_(){
	
	
	while(!(UCSR0A & (1<<RXC0))){
		Wait(1);
	}
	
	return UDR0;
}

void write(char val){
	
	while(!(UCSR0A & (1<<UDRE0))){
	}
	
	UDR0=val;		
}

