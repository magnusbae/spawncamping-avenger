/*
 * byggern.c
 *
 * Created: 29.08.2013 12:12:10
 *  Author: tommp
 */ 


#include <avr/io.h>

int main(void)
{
	DDRB |= (1 << DDB1);
    while(1)
    {
		PORTB |= (1 << DDB1);
		PORTB &= ~(1 << DDB1);
        //TODO:: Please write your application code 
    }
}