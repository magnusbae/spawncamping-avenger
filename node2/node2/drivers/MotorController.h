/*
 * MotorController.h
 *
 * Created: 07.11.2013 15:07:36
 *  Author: magnussb
 */ 


#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

#include <avr/io.h>
#include "joyCan.h"

#define DAC_ADDRESS	0b01011110
#define DAC_0_OUTPUT 0b00000000
#define DAC_OUTPUT_REGISTER_RESET 0b00010000

#define MOTOR_CONTROLLER_PORT PORTC
#define MOTOR_ENABLE PC6
#define MOTOR_DIRECTION PC7
#define MOTOR_ENCODER_ENABLE_ACTIVE_LOW PC3
#define MOTOR_ENCODER_RESET_ACTIVE_LOW PC4
#define MOTOR_ENCODER_SELECT_HI_OR_LOW_BYTE PC5


void initialMotorControlSetup();
void setMotorPowerFromJoystickPosition(joystickPosition jp);
void disableMotor();

#endif /* MOTORCONTROLLER_H_ */