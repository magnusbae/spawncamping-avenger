#include "MotorController.h"
#include "joyCan.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <util/delay.h>

unsigned char messageBuf[4];
uint8_t isInvertedOutput = 0;
int time_stamp = 0; //needs to be set once a reference change is detected



void disableMotor();
void enableMotor();
void setMotorEnabledState(uint8_t shouldEnable);
void setMotorDirection();
void setDac0Output(uint8_t valueFrom0To255);
uint8_t calculateByteValue(uint8_t joystickValue);
uint8_t readEncoderValue();
void resetEncoder();
void initializeEncoder();

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
	if(isInvertedOutput){
		MOTOR_CONTROLLER_PORT |= (1<<MOTOR_DIRECTION);
	}else{
		MOTOR_CONTROLLER_PORT &= ~(1<<MOTOR_DIRECTION);
	}
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
		isInvertedOutput = 0;
		setDac0Output(calculateByteValue(pos));
	}else{
		pos = 50-pos;
		isInvertedOutput = 1;
		setDac0Output(calculateByteValue(pos));
	}
}

uint8_t calculateByteValue(uint8_t joystickValue){
	uint8_t retval = ((float)(joystickValue)*4);
	return retval;
}

void initializeEncoder(){
	resetEncoder();
}

uint8_t readEncoderValue(){
	MOTOR_CONTROLLER_PORT &= ~(1<<MOTOR_ENCODER_OUTPUT_ENABLE_ACTIVE_LOW) | ~(1<<MOTOR_ENCODER_SELECT_HI_OR_LOW_BYTE);
	_delay_ms(1);
	volatile uint8_t val_high = ENCODER_PINS;
	MOTOR_CONTROLLER_PORT |= (1<<MOTOR_ENCODER_SELECT_HI_OR_LOW_BYTE);
	_delay_ms(1);
	volatile uint8_t val_low = ENCODER_PINS;
	MOTOR_CONTROLLER_PORT |= (1<<MOTOR_ENCODER_OUTPUT_ENABLE_ACTIVE_LOW);
	printf("Read value: %02x %02x\r\n", val_low, val_high);
	return val_low;
}

void resetEncoder(){
	MOTOR_CONTROLLER_PORT &= ~(1<<MOTOR_ENCODER_OUTPUT_ENABLE_ACTIVE_LOW) | ~(1<<MOTOR_ENCODER_RESET_ACTIVE_LOW);
	_delay_ms(1);
	MOTOR_CONTROLLER_PORT |= (1<<MOTOR_ENCODER_RESET_ACTIVE_LOW) | (1<<MOTOR_ENCODER_OUTPUT_ENABLE_ACTIVE_LOW);
}

uint8_t convertEncoderValue(uint8_t convme){
	//convert and return
}

void regulator(inputMessage data){
	//might not need PID, depending on motor PI might be enought (or even P)
	
	uint8_t diff = 0;
	uint8_t voltage = 0;
	uint8_t position = convertEncoderValue(readEncoderValue());
	uint8_t reference = data.motorPosition;
	uint8_t deltatime = 0; //replace with time since time_stamp or use a timer module from the avr (we want the time since the refrence was changed)
	//float speed = 0; //need a function for calculating speed ((mesurement1-mesurement2)/(deltatimebetween mesurements))
	
	if (position>reference+5){
		isInvertedOutput=0;
		diff = position-reference;
	}
	else if(position<reference-5){
		isInvertedOutput=1;
		diff = reference-position;
	}
	else{
		time_stamp = 0; //reset it
	}
	
	voltage = Kp*diff + Ki*diff*deltatime; // - Kp*speed;
	
	
	setMotorDirection();
	setDac0Output(voltage);
}