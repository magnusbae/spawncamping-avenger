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

void initializeADC();
int adc_readvalue();
int game_CheckBallDropped();


#endif /* ADC128_H_ */