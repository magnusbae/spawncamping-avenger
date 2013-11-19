/*
 * multifunction.h
 *
 * Created: 19.09.2013 16:41:43
 *  Author: tommp
 */ 

#ifndef MULTIFUNCTION_H_
#define MULTIFUNCTION_H_


#define ADC_ADRESS 0x1400
#define ADC_CHANNEL1 0b00000100
#define ADC_CHANNEL2 0b00000101
#define ADC_CHANNEL3 0b00000110
#define ADC_CHANNEL4 0b00000111

#define JOYSTICK_X ADC_CHANNEL1
#define JOYSTICK_Y ADC_CHANNEL2
#define IR_DIODE ADC_CHANNEL3
#define TOUCH_RIGHT ADC_CHANNEL4

#define DIRECTION_TRESHOLD_NEGATIVE 110
#define DIRECTION_TRESHOLD_POSITIVE 150
#define DIRECTION_NEUTRAL 132

#include <avr/io.h>

/**
Holds the x and y coordinates of the joystick
50 is the middle value
*/
typedef struct
{
	char xPosition;
	char yPosition;
} joystickPosition;

joystickPosition jp_global;

/**
	Holds the direction of the joystick from the enumerated list.
*/
typedef struct
{
	enum {LEFT, RIGHT, UP, DOWN, NEUTRAL} direction;
} joystickDirection;

/**
* Reads the position of the joystick
* Introduces > 2ms delay while waiting for ADC.
@retval joystickPosition
*/
joystickPosition readJoystickPosition();

/**
* Reads the direction of the joystick
* Introduces > 2ms delay while waiting for ADC.
@retval joystickDirection
*/
joystickDirection readJoystickDirection();

/**
* Reads the value from the IR-diode. Not really what it was used for originally, but the ADC on the STK500/501 
* wasn't really helpful. Whenever we turned on the ref-voltages it managed to increase the voltage on the board. 
* Didn't give any values when we turned off reference voltage.
@retval The ADC value.
*/
uint8_t readIrDiode();

/**
Reads the position of the right touch slider
@retval 0 if the position is all the way to the left
@retval 255 if the position is all the way to the right.
@retval 0-255 for all other positions
*/
uint8_t readRightSlider();

/**
Reads an ADC input, no calibration
@param [in] input The 8-bit address of the ADC-input
@retval int The converted value from the ADC input port on the given address.
*/
uint8_t readAdcInput(uint8_t input);

#endif /* MULTIFUNCTION_H_ */