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
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "../fonts/font_4x6.h"

int col_number=0;
int line_number=0;
int ramcurrent = 0x1800;


void init_program();

void oled_clear_display() 
{	
	for(int i = 0; i < 8; i++){
		oled_clear_line(i);
	}
}


void oled_printf(char* line){
		
	int i = 0;
	while((int)line[i] != EOL){
		if(col_number >= DISPLAY_WIDTH){
			col_number = 0;
			line_number == 7 ? line_number = 0 : line_number++;
			oled_goto_position(line_number, 0);
			oled_clear_line(line_number);
		}
		if ((int)line[i]==NEWLINE){
			if (line_number==7){
				line_number=0;
			}
			else{
				line_number++;
			}			
			oled_goto_position(line_number, 0);
			oled_clear_line(line_number);
		}else{
			oled_print(line[i]);
			col_number++;
		}
		i++;
	}
}

void init_oled(){
	init_program();
	oled_clear_display();	
	oled_home();	
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
	oled_goto_position(0,0);
}

void oled_goto_column(int column){
	col_number = column;
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
	line_number = line;
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

void oled_ramstore(char* line){
	volatile char *ext_ram = (char *) ramcurrent;
	unsigned int endofstring=0;
	for (int i = 0; i<(128/CHARACTER_WIDTH); i++){
		if (line[i]==EOL){
			endofstring=1;
		}								
		if (!endofstring){
			for (int j = 0; j < CHARACTER_WIDTH; j++){
				ext_ram[ramcurrent] = pgm_read_byte(&font[((int)line[i])-32][j]);
				ramcurrent++;
				if (ramcurrent==RAMEND){
					ramcurrent = RAMSTART;
				}
			}
		}
		else{
			for (int j = 0; j < CHARACTER_WIDTH; j++){
				ext_ram[ramcurrent] = 0b00000000;
				ramcurrent++;
			}				
			if (ramcurrent==RAMEND){
				ramcurrent = RAMSTART;
			}
		}
	}
}											

void oled_ramclear(){
	volatile char *ext_ram = (char *) RAMSTART;
	unsigned int ramarea = 0x400;//RAMEND-RAMSTART;
	for (int i = 0; i < ramarea; i++){
		ext_ram[i] = 0b00000000;
	}
}

void oled_ramgotopos(int line, int column){
	ramcurrent=127*line + column;
}

void oled_ramtransefer(){
	volatile char *ext_ram = (char *) RAMSTART;
	volatile char *oled = (char *) OLED_DATA;
	unsigned int temp = 0b0;
	unsigned int ramarea = 0x400;//RAMEND-RAMSTART;
	for (int i = 0; i<(ramarea); i++){
		if (i%124==0){
			line_number++;
			oled_goto_position(line_number, col_number);
		}
		temp = ext_ram[i];
		oled[0] = temp;
	}
}