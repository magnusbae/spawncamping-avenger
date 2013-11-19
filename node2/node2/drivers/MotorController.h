/*
 * MotorController.h
 *
 * Created: 07.11.2013 15:07:36
 *  Author: magnussb
 */ 


#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
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

//needs tuning
#define Kp 3
#define Kd 18
#define Ki 3


#define MOTOR_SLOW_SPEED 100
#define MOTOR_POSITION_CENTER 127


/**
Sets up the input and output ports for the motor controller (motor box)
*/
void initialMotorControlSetup();

/**
Inits the encoder
*/
void initializeEncoder();

/**
@deprecated{Superseeded by timer and regulate function}
Read an inputMessage and set motorpower and direction
*/
void setMotorPowerFromInputData(inputMessage jp);

/**
Turn off the enable pin on the motor box
*/
void disableMotor();

/**
Turn on the enable pin on the motor box
*/
void enableMotor();

/**
Read length of board from right to left and set startup values
Then turn on timed regulation to position carriage in the middle
*/
void calibrateMotor();

/**
Read the 16 bit encoder value. Inserts a 50µs wait between register reads.
*/
int readEncoderValue();

/**
Sets the output to the motor using a DAC. Gives 0-5V output based on 8-bit input
@param [in] valueFrom0To255 The 8-bit value to use to give DC voltage on the motor box. 255 = 5V
*/
void setDac0Output(uint8_t valueFrom0To255);

/**
Resets the encoder registers to 0
*/
void resetEncoder();

/**
Sets the motor direction state. 
@param [in] right Set to 1 if motor should go right, 0 for left.
*/
void setMotorDirectionInput(uint8_t right);

/**
Do some fancy movements for extra credit
*/
void showboat();

/**
Set internal state inputMessage for the timed regulator.
This ensures updated values are ready when the timer triggers
@param [in] nextObjective The last received inputMessage containing the latest instructions for gameplay.
*/
void setReceivedInputDataMessage(inputMessage nextObjective);



#endif /* MOTORCONTROLLER_H_ */