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

typedef struct
{
	char xPosition;
	char yPosition;
} joystickPosition;

joystickPosition jp_global;

typedef struct
{
	enum {LEFT, RIGHT, UP, DOWN, NEUTRAL} direction;
} joystickDirection;

/*
* Introduces > 2ms delay while waiting for ADC.
*/
joystickPosition readJoystickPosition();
joystickDirection readJoystickDirection();
uint8_t readIrDiode();
uint8_t readRightSlider();
uint8_t readAdcInput(uint8_t input);

#endif /* MULTIFUNCTION_H_ */