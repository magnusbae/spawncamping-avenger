/*
 * Timer.h
 *
 * Created: 19.11.2013 16:05:45
 *  Author: tommp
 */ 


#ifndef TIMER_H_
#define TIMER_H_

/**
Sets up timer1 on the Atmel 162 MCU
*/
void setupTimer();

/**
Resets timer1 on the Atmel 162 MCU
*/
void resetTimer();

/**
Disables timer1 on the Atmel 162 MCU
*/
void destroyTimer();


#endif /* TIMER_H_ */