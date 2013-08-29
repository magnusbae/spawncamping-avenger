/*
 * byggern.c
 *
 * Created: 29.08.2013 12:12:10
 *  Author: tommp
 */ 

#include <util/delay.h>
#include <avr/io.h>

	void Wait(int waitTime)
	{
		for(uint8_t i=0;i<waitTime;i++)
		{
			_delay_loop_2(0);
		}
	}
	
int main(void)
{
	DDRB = 0xFF;
	Wait(10);
	PORTB = 0x80;
    while(1)
    {
		for (int i = 0; i < 8; i++){
			PORTB = (PORTB >> 1);
			
			if(i == 0){
				PORTB |= (1 << DDB7);
			}
			Wait(2);
		}
		
		
        //TODO:: Please write your application code 
    }
}