/*
 * PWMdriver.c
 *
 * Created: 31.10.2013 15:46:19
 *  Author: magnussb
 */ 

#define SIGNAL_PERIOD 20000
#define PULSE_CENTER 1500
#define PULSE_MAX 2100
#define PULSE_MIN 1900


void initializePWM(){
//WGM01:0 = 3 sets to PWM fast slope mode, we want non-inverting Compare Output mode (COM01:0 bits to 2) that counts from 0 to 20000 ns (20 ms, SIGNAL_PERIOD) and clears OC0  on match with adjusted joy value(OCR0), TCNT0 is the counter value
//OC0 must be set to output
	//TODO:Set oc0 as output, CS0:2 must make the period equal to 20 ms(SIGNAL_PERIOD)
	
	TCCR0 |= ((WGM01<<1) | (WGM00<<1) | (COM01<<1));
}

