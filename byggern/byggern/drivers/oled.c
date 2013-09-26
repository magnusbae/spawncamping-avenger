/*
 * oled.c
 *
 * Created: 26.09.2013 12:38:44
 *  Author: magnussb
 */ 

/*
	
	switch D and C
	address OLED
	write data
	switch D and C

*/

#include "oled.h"
#include <avr/io.h>
#include "../fonts/font_4x6.h"

void init_program();

void init_oled(){
	init_program();
	//oled_print('f');	
	for(int i = 0; i < 64; i++){
		oled_clear_line(i);
	}
}

void init_program()
{	
	write_command(0xae); // display off
	write_command(0xa1); //segment remap
	write_command(0xda); //common pads hardware: alternative
	write_command(0x12);
	write_command(0xc8); //common output scan direction:com63~com0
	write_command(0xa8); //multiplex ration mode:63
	write_command(0x3f);
	write_command(0xd5); //display divide ratio/osc. freq. mode
	write_command(0x80);
	write_command(0x81); //contrast control
	write_command(0x50);
	write_command(0xd9); //set pre-charge period
	write_command(0x21);
	write_command(0x20); //Set Memory Addressing Mode
	write_command(0x02);
	write_command(0xdb); //VCOM deselect level mode
	write_command(0x30);
	write_command(0xad); //master configuration
	write_command(0x00);
	write_command(0xa4); //out follows RAM content
	write_command(0xa6); //set normal display
	write_command(0xaf); // display on
}

void oled_clear_line(int line){
	write_command(0x20);
	write_command(0x00);

	write_command(0x21);
	write_command(1);
	write_command(127);
	

	for(int i = 0; i < 127; i++){
		volatile char *oled_clear = OLED_DATA;
		oled_clear[0] = 0xff;
	}
}

void oled_print(char* c){
	volatile char *oled = (char *) OLED_DATA;
	int i = 0;
	for (int i = 0; i < 4; i++){
		oled[0] = font[(int)c][i];	
	}
}

void write_command(char c){
	
	volatile char *oled = (char *) OLED_COMMAND;
	oled[0] = c;
}

