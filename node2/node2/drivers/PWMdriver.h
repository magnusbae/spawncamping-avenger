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
#define PULSE_CENTER 1500
#define PULSE_MAX 2100
#define PULSE_MIN 900

void initializePWM();
void set_servopos(joystickPosition joypos);



#endif /* PWMDRIVER_H_ */