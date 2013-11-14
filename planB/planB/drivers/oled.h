/*
 * oled.h
 *
 * Created: 26.09.2013 13:48:46
 *  Author: magnussb
 */ 


#ifndef OLED_H_
#define OLED_H_

#define OLED_COMMAND 0x8000
#define OLED_DATA 0x8200
#define CHARACTER_WIDTH 4
#define EOL 0
#define NEWLINE 10
#define DISPLAY_WIDTH 32
#define RAMSTART 0x1800
#define RAMEND 0x1BFF


void init_oled();
void write_command(char c);
void oled_clear_line(int line);
void oled_clear_display();
void oled_print(char* c);
void oled_home();
void oled_goto_position(int row, int column);
void oled_goto_line(int line);
void oled_clear_char(int row, int startPos);
void oled_printf(char* line);
void oled_ramstore(char* line);
void oled_ramclear();
void oled_ramgotopos(int line, int column);
void oled_ramtransfer();


#endif /* OLED_H_ */