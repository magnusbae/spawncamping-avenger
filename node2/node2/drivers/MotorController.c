#include "MotorController.h"
#include "joyCan.h"
#include "TWI_Master.h"
#include <avr/io.h>

unsigned char messageBuf[4];
uint8_t isInvertedOutput = 0;

void enableMotor(uint8_t shouldEnable);
void setMotorDirection();
void setDac0Output(uint8_t valueFrom0To255);
uint8_t calculateByteValue(uint8_t joystickValue);

void initialMotorControlSetup(){
	DDRC = 0xFF;
	TWI_Master_Initialise();
	MOTOR_CONTROLLER_PORT |= (1<<MOTOR_ENCODER_ENABLE_ACTIVE_LOW) | (1<<MOTOR_ENCODER_RESET_ACTIVE_LOW);
	enableMotor(1);
}

void disableMotor(){
	enableMotor(0);
}

void enableMotor(uint8_t shouldEnable){
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
   //setMotorDirection();
   MOTOR_CONTROLLER_PORT |= (1<<MOTOR_DIRECTION);
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

void setMotorPowerFromJoystickPosition(joystickPosition jp){
	uint8_t pos = jp.xPosition;
	if(pos == 50){
		setDac0Output(0);
	}else if(pos > 50){
		pos -= 50;
		isInvertedOutput = 1;
		setDac0Output(calculateByteValue(pos));
	}else{
		pos = 50-pos;
		isInvertedOutput = 0;
		setDac0Output(calculateByteValue(pos));
	}
}

uint8_t calculateByteValue(uint8_t joystickValue){
	uint8_t retval = ((float)(joystickValue)*4);
	return retval;
}