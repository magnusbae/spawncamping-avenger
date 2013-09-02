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
	fdevopen(write, read_);
    while(1)
    {
		
		char a = read_();
		if(a == 'p'){
			write(a);
		}else{
			printf("Nothing to see here ");
		}
		
		
		
			        
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

