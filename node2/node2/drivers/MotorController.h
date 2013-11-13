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

#define MOTOR_CONTROLLER_DDR DDRC
#define MOTOR_CONTROLLER_DDR_VALUES 0xff
#define MOTOR_CONTROLLER_PORT PORTC

#define MOTOR_ENABLE PC4
#define MOTOR_DIRECTION PC3
#define MOTOR_ENCODER_OUTPUT_ENABLE_ACTIVE_LOW PC7
#define MOTOR_ENCODER_RESET_ACTIVE_LOW PC6
#define MOTOR_ENCODER_SELECT_HI_OR_LOW_BYTE PC5


#define ENCODER_DDR DDRA
#define ENCODER_DDR_VALUES 0x00
#define ENCODER_PINS PINA

#define ENCODER0 PA7
#define ENCODER1 PA6
#define ENCODER2 PA5
#define ENCODER3 PA4
#define ENCODER4 PA3
#define ENCODER5 PA2
#define ENCODER6 PA1
#define ENCODER7 PA0

//needs tuning (pref. ziegler nichols)
#define Kp 1
#define Kd 0
#define Ki 0


#define MOTOR_SLOW_SPEED 100


void initialMotorControlSetup();
void initializeEncoder();
void setMotorPowerFromInputData(inputMessage jp);
void disableMotor();
void calibrateMotor();
int readEncoderValue();

#endif /* MOTORCONTROLLER_H_ */