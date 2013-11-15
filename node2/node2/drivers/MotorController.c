#include "crystal.h"
#include "MotorController.h"
#include "joyCan.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//"private" methods
unsigned char bitReverse(unsigned char x);
uint8_t readEncoderPins();
uint8_t calculateByteValue(uint8_t joystickValue);
float calculateSpeed();
float calculateErrorSpeed(inputMessage data);
void setMotorEnabledState(uint8_t shouldEnable);
void setupTimer();
void destroyTimer();

//--
unsigned char messageBuf[4];
volatile uint8_t isDirectionRight = 0;
volatile int lastReadEncoderValue = 0;
volatile int encoderMaxValue = 0;
uint8_t errorPile = 0; //Will we actually use this one?

uint8_t isTimerSetUp = 0;
uint8_t assumedMotorPosition = 0;
inputMessage inputCommand;

void setupTimer(){
	isTimerSetUp = 1;
	TCCR3B |= (1<<CS32) | (1<<CS30); //Start timer 3, f(clk_io)/1024 prescaling
	ETIMSK |= (1<<TOIE3); //Enables interrupt on timer 3
}

void destroyTimer(){
	isTimerSetUp = 0;
	TCCR3B &= ~(1<<CS32) | ~(1<<CS30); //Stop timer 3
	ETIMSK &= ~(1<<TOIE3); //Disables interrupt on timer 3
	
}

void setReceivedInputDataMessage(inputMessage nextObjective){
	inputCommand = nextObjective;
}

void setDirectionLeft(){
	isDirectionRight = 0;
}

void setDirectionRight(){
	isDirectionRight = 1;
}

void initialMotorControlSetup(){
	MOTOR_CONTROLLER_DDR = MOTOR_CONTROLLER_DDR_VALUES;
	ENCODER_DDR = ENCODER_DDR_VALUES;
	TWI_Master_Initialise();
	MOTOR_CONTROLLER_PORT |= (1<<MOTOR_ENCODER_OUTPUT_ENABLE_ACTIVE_LOW) | (1<<MOTOR_ENCODER_RESET_ACTIVE_LOW);
	enableMotor();
}

void disableMotor(){
	setMotorEnabledState(0);
}

void enableMotor(){
	setMotorEnabledState(1);
}

void setMotorEnabledState(uint8_t shouldEnable){
	if(shouldEnable){
		MOTOR_CONTROLLER_PORT |= (1<<MOTOR_ENABLE);
	}else{
		MOTOR_CONTROLLER_PORT &= ~(1<<MOTOR_ENABLE);
	}
}

void setMotorDirection(){
	if(isDirectionRight){
		MOTOR_CONTROLLER_PORT |= (1<<MOTOR_DIRECTION);
	}else{
		MOTOR_CONTROLLER_PORT &= ~(1<<MOTOR_DIRECTION);
	}
}

void calibrateMotor(){
	cli();
	
	resetEncoder();
	setDirectionRight();
	setDac0Output(MOTOR_SLOW_SPEED);
	_delay_ms(50);
	//Run right until we hit the wall
	while(calculateSpeed()>15){
		lastReadEncoderValue = readEncoderValue();
	}
	setDac0Output(0);
	resetEncoder();

	setDirectionLeft();
	setDac0Output(MOTOR_SLOW_SPEED);
	//Run left until we hit the wall
	while(calculateSpeed()>15){
		encoderMaxValue = readEncoderValue();
	}
	setDac0Output(0);//STOP! Hammertime!

	setDirectionRight();
	setDac0Output(MOTOR_SLOW_SPEED);
	volatile int target = encoderMaxValue/2;
	while(readEncoderValue() > target){
		
		} //empty
	setDac0Output(0);
	//Motor calibrated and centered!

}


void setDac0Output(uint8_t valueFrom0To255){
   setMotorDirection();
   char TWI_targetSlaveAddress = DAC_ADDRESS;
   messageBuf[0] = (TWI_targetSlaveAddress) | (FALSE<<TWI_READ_BIT); // The first byte must always consist of General Call code or the TWI slave address.
   messageBuf[1] = DAC_0_OUTPUT;             // The first byte is used for commands.
   messageBuf[2] = valueFrom0To255;                        // The second byte is used for the data.
   TWI_Start_Transceiver_With_Data( messageBuf, 3 );
   
   while(TWI_Transceiver_Busy()){
	   //empty
   }
   if ( ! TWI_Transceiver_Busy() )
    {
	    // Check if the last operation was successful
	    if ( TWI_statusReg.lastTransOK )
	    {
		    //printf("Message successfully sent to DAC");
	    }
	    else // Got an error during the last transmission
	    {
		    // Use TWI status information to detemine cause of failure and take appropriate actions.
		    unsigned char state = TWI_Get_State_Info();
		    printf("TWI state %02x\r\n", state);
	    }
    }
}

void setMotorPowerFromInputData(inputMessage jp){
	uint8_t pos = jp.motorPosition;
	if(pos == 50){
		setDac0Output(0);
	}else if(pos > 50){
		pos -= 50;
		isDirectionRight = 0;
		setDac0Output(calculateByteValue(pos));
	}else{
		pos = 50-pos;
		isDirectionRight = 1;
		setDac0Output(calculateByteValue(pos));
	}
}

uint8_t calculateByteValue(uint8_t joystickValue){
	uint8_t retval = ((float)(joystickValue)*4);
	return retval;
}

void initializeEncoder(){
	MOTOR_CONTROLLER_PORT &= ~(1<<MOTOR_ENCODER_OUTPUT_ENABLE_ACTIVE_LOW);
	resetEncoder();
}

uint8_t readEncoderPins(){
	unsigned char valueReadFromPort = ENCODER_PINS;
	return bitReverse(valueReadFromPort);
}

unsigned char bitReverse( unsigned char x )
{
	//Copied from the internet (somewhere)
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	return x;
}

int readEncoderValue(){
	cli();
	MOTOR_CONTROLLER_PORT &= ~(1<<MOTOR_ENCODER_SELECT_HI_OR_LOW_BYTE);
	_delay_us(25);
	volatile uint8_t val_high = readEncoderPins();
	MOTOR_CONTROLLER_PORT |= (1<<MOTOR_ENCODER_SELECT_HI_OR_LOW_BYTE);
	_delay_us(25);
	volatile uint8_t val_low = readEncoderPins(); //ENCODER_PINS;
	sei();
	return (val_high*0b100000000) + val_low;
}

void resetEncoder(){
	MOTOR_CONTROLLER_PORT &=  ~(1<<MOTOR_ENCODER_RESET_ACTIVE_LOW);
	_delay_ms(10);
	MOTOR_CONTROLLER_PORT |= (1<<MOTOR_ENCODER_RESET_ACTIVE_LOW);
}

uint8_t convertEncoderValue(int convme){
	int factor = encoderMaxValue / 255;
	uint8_t returnme = convme/factor;
	if (returnme>255){
		return (uint8_t)255;
	}
	else if(returnme<0){
		return (uint8_t)0;
	}
	else{
		return (uint8_t)returnme;
	}
}

float calculateSpeed(){
	float mesurement_1=abs(readEncoderValue());
	_delay_ms(5);
	float mesurement_2=abs(readEncoderValue());
	return (abs(mesurement_2-mesurement_1))/205.00;
}

float calculateErrorSpeed(inputMessage data){
	uint8_t refr = 255-data.motorPosition;
	float error1 = abs(convertEncoderValue(readEncoderValue()))-refr;
	_delay_ms(5);
	float error2 = abs(convertEncoderValue(readEncoderValue()))-refr;
	
	return ((abs(error2-error1))/205.00)*200;
	
}

void regulator(){
	//might not need PID, depending on motor PI might be enought (or even P)
	
	uint8_t error = 0;
	uint8_t voltage = 0;
	uint8_t position = convertEncoderValue(readEncoderValue());
	uint8_t reference = 255-inputCommand.motorPosition;
	uint8_t deltatime = 0; //replace with time since time_stamp or use a timer module from the avr (we want the time since the refrence was changed)
	
	if (position>reference+5){
		setDirectionRight();
		error = position-reference;
	}
	else if(position<reference-5){
		setDirectionLeft();
		error = reference-position;
	}
	else{
		errorPile = 0;
		error = 0;
	}
	
	uint8_t errorSpeed = calculateErrorSpeed(inputCommand);
	
	errorPile += error;
	
	voltage = Kp*error +Ki*errorPile - Kd*errorSpeed;
	
	//printf("Volt: %d Diff: %d ", prev_voltage, diff);
	if(voltage>255){
		voltage = 255;
	}
	
	if (voltage<0){
		voltage = 0;
	}
	
	setDac0Output(voltage);
}


//ISR(TOIE3_vect) TODO find right vector and implement