/*
 * planB.c
 *
 * Created: 14.11.2013 11:54:16
 *  Author: magnussb
 */ 

#include "drivers/crystal.h"
#include <avr/io.h>
#include <avr/delay.h>

#include "drivers/addressBus.h"
#include "drivers/oled.h"
#include "menu/menu.h"
#include "drivers/CanMessaging.h"
#include "drivers/MCP2515.h"
#include "drivers/SPI.h"

volatile uint8_t receivedMessageFlag = 0;
volatile canMessage receivedCanMessage;

int main(void)
{
		
	initialAddressBusSetup();
	SPI_MasterInit();
	mcp_init();
	
	_delay_ms(250);
	init_oled();
	
	DDRD |= (1<<PD0);
	
	

	oled_printf("Boot complete!");
	
	uint8_t menuchanged = 1;
	uint8_t menuLenght = 4;
	menuOption menu[menuLenght];
	menu[0].isSelected = SELECTED;
	menu[0].name = "Menyvalg 1";
	menu[1].name = "Menyvalg 2";
	menu[2].name = "Menyvalg 3";
	menu[3].name = "Menyvalg 4";
	
	canMessage message;
	message.identifier = 1;
	message.RTR = 0;
	message.extendedFrame = 0;
	message.length = 1;
	message.data[0] = 'j';
	if(CAN_send_message(message)){
		oled_printf("\nCan message sent");
	}
	
	//menu_display(menu, menuLenght, &menuchanged);
    while(1)
    {
		PORTD ^= (1<<PD0);
		_delay_ms(100);
        
		
		if(receivedMessageFlag){
			oled_printf("\nReceived CAN-message.");
			receivedMessageFlag = 0;
			receivedCanMessage = CAN_read_received_message();
			mcp_clear_interrupt();
		/*	printf("Received CAN message with data length %d", receivedMessage.length);
			for (int i = 0; i < receivedCanMessage.length; i++){
				printf(" %d", receivedCanMessage.data[i]);
			}*/
			
			oled_printf("\nLength: ");
			oled_print(receivedCanMessage.length);
			oled_printf("\nContent: ");
			oled_print(receivedCanMessage.data[0]);
			
			if(receivedCanMessage.data[0] == 's'){
				/*oled_ramgotopos(5,0);
				oled_ramstore("Game score: -");
				char buffer[3];
				sprintf( buffer, "%d", receivedMessage.data[1] );
				oled_ramgotopos(5,13*4);
				oled_ramstore(buffer);
				oled_ramtransfer();*/
			}
		}
    }
}


ISR(INT1_vect){
	receivedMessageFlag = 1;	
}