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
    while(1)
    {
		//while(!(UCSR0A & (1<<RXC0))){
		//}
		write('a');
		//while(!(UCSR0A & (1<<TXC0))){
		//}
        read_();
    }
}

void set_initial(uint8_t baud_rate_value){
	
	
	UBRR0H=(8>>baud_rate_value);
	UBRR0L=baud_rate_value;
	UCSR0C|=(1<<URSEL0)|(1<<UCSZ01)|(1<<UCSZ00);
	UCSR0B|=(1<<RXEN0)|(1<<TXEN0);
	UCSR0B|=(1<<RXCIE0)|(1<<TXCIE0);
}

char read_(){
	
	
	return UDR0;
}

void write(char val){

	
	UDR0=val;		
}