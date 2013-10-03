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
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "../fonts/font_4x6.h"

void init_program();

void clear_display() 
{	
	for(int i = 0; i < 8; i++){
		oled_clear_line(i);
	}
}

void init_oled(){
	init_program();
	
	clear_display();		
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

void oled_home(){
	//Goes to top left coordinate
	write_command(0xB0);
	write_command(0x00);
	write_command(0x10);
}

void oled_goto_column(int column){
	uint8_t lsb = column & 0x0F; 
	uint8_t msb = (column & 0xF0) / 0b10000;
	write_command(lsb);
	write_command(0x10 | msb);
}

void oled_goto_position(int row, int column){
	oled_goto_line(row);
	oled_goto_column(column);
}

void oled_goto_line(int line){
	//char-lines, 0-7
	write_command(0xB0 + line);
}

void oled_clear_line(int line){
	oled_goto_position(line,0);
	for(int i = 0; i < 128; i++){
		volatile char *oled_clear = OLED_DATA;
		oled_clear[0] = 0x00;
	}
}

void oled_print(char* c){
	volatile char *oled = (char *) OLED_DATA;
	for (int i = 0; i < CHARACTER_WIDTH; i++){
		oled[0] = pgm_read_byte(&font[((int)c)-32][i]);
	}
}


void write_command(char c){
	
	volatile char *oled = (char *) OLED_COMMAND;
	oled[0] = c;
}

