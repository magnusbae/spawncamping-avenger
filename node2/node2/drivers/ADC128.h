/*
 * ADC128.h
 *
 * Doesn't work with our STK500/501! Don't use
 * Setting an internal VREF creates overvoltage on the STK, makes the motorbox beep and 
 * in general makes everybody have a very bad day. Use the free ADC-port on node 1 instead.
 * 
 */ 


#ifndef ADC128_H_
#define ADC128_H_

#define TRIGGERVALUE 100

/**
Initializes the ADC on the STK501 top board. 
Also increases the board voltage to between 5,6 and 5,9 volts depending on which internal (infernal?) 
voltage refernce is chosen
*/
void initializeADC();

/**
Pretends to read an ADC value, but if we want to keep components alive it really doesn't 
*/
int adc_readvalue();

/**
Should return 1 if the ball is dropped. See node1 instead
*/
int game_CheckBallDropped();


#endif /* ADC128_H_ */