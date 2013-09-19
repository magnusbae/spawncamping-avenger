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
#define TOUCH_LEFT ADC_CHANNEL3
#define TOUCH_RIGHT ADC_CHANNEL4

typedef struct  
{
	unsigned int xPosition;
	unsigned int yPosition;
} joystickPosition;

volatile char *adc = (char *) 0x1400;

/*
* Introduces > 2ms delay while waiting for ADC.
*/
joystickPosition readJoystickPosition();
int readLeftSlider();
int readRightSlider();

#endif /* MULTIFUNCTION_H_ */