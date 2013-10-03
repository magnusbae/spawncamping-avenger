/*
 * oled.h
 *
 * Created: 26.09.2013 13:48:46
 *  Author: magnussb
 */ 


#ifndef OLED_H_
#define OLED_H_

#define OLED_COMMAND 0x1000
#define OLED_DATA 0x1200
#define CHARACTER_WIDTH 4
#define EOL 0
#define NEWLINE 10
#define DISPLAY_WIDTH 32

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


#endif /* OLED_H_ */