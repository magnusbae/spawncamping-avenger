/*
 * uart.c
 *
 * Created: 19.09.2013 16:23:48
 *  Author: tommp
 */ 

#include "uart.h"
#include <avr/io.h>
#include <stdio.h>
#include "crystal.h"
#include <util/delay.h>

void setBaudRateAndInitializeUartWithCorrectBits(uint8_t baud_rate_value){
	UBRR0H=(8>>baud_rate_value);
	UBRR0L=baud_rate_value;
	UCSR0C|=((1<<USBS0)|(3<<UCSZ0));
	UCSR0B|=(1<<RXEN0)|(1<<TXEN0);
	//UCSR0B|=(1<<RXCIE0)|(1<<TXCIE0); //Don't use this. Toggles interrupt
}

void Wait(int waitTime)
{
	for(uint8_t i=0;i<waitTime;i++)
	{
		_delay_loop_2(0);
	}
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

void printWelcomeMessage(){
	printf("\r\n");
	printf("__________                                          \r\n");
	printf("\\______   \\___.__. ____   ____   ___________  ____  \r\n");
	printf(" |    |  _<   |  |/ ___\\ / ___\\_/ __ \\_  __ \\/    \\ \r\n");
	printf(" |    |   \\\\___  / /_/  > /_/  >  ___/|  | \\/   |  \\ \r\n");
	printf(" |______  // ____\\___  /\\___  / \\___  >__|  |___|  /\r\n");
	printf("        \\/ \\/   /_____//_____/      \\/           \\/ \r\n");
}

void setupUartAndSendWelcomeMessage()
{
	setBaudRateAndInitializeUartWithCorrectBits(BAUD_RATE_4_9MHZ);
	fdevopen(write, read_);
	printWelcomeMessage();
	printf("UART Initialized\r\n");
}
