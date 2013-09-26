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

void init_oled();
void write_data(char c);
void write_command(char c);

#endif /* OLED_H_ */