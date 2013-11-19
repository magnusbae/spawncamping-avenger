/*
 * PWMdriver.h
 *
 * Created: 31.10.2013 15:46:39
 *  Author: magnussb
 */ 


#ifndef PWMDRIVER_H_
#define PWMDRIVER_H_

#include "joyCan.h"

#define SIGNAL_PERIOD 20000
#define PULSE_MAX 2184
#define PULSE_MIN 936


/**
Initializes the PWM to default state to drive the servo on the gameboard.
*/
void initializePWM();

/**
Adjust the PWM signal bases on the received inputMessage
@param [in] joypos The received inputMessage containing a position for the servo between 0 and 100
*/
void set_servopos(inputMessage joypos);



#endif /* PWMDRIVER_H_ */