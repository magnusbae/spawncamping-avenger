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
#define RAMSTART 0x1800
#define RAMEND 0x1BFF

/**
Initializes the OLED.
*/
void init_oled();
void write_command(char c);

/**
Clears a line on the OLED display (6 pixels high)
@param [in] line The line to clear (0-7)
*/
void oled_clear_line(int line);

/**
Clears the entire display
*/
void oled_clear_display();

/**
Prints one character on the display
@param [in] c the character to print
*/
void oled_print(char* c);

/**
Returns the cursor to origo (upper left corner)
*/
void oled_home();

/**
Goes to the given coordinate
@param [in] row The row to go to
@param [in] column The column to go to
*/
void oled_goto_position(int row, int column);

/**
Go the start of the given line
@param [in] line The line to go to (0-7)
*/
void oled_goto_line(int line);
void oled_clear_char(int row, int startPos);

/**
Very simple function to print strings to the OLED
@param [in] line The string of characters to print
*/
void oled_printf(char* line);

/**
Stores a string in SRAM (OLED)
@param [in] line The string to store
*/
void oled_ramstore(char* line);

/**
Clears the SRAM (OLED)
*/
void oled_ramclear();

/**
Go to the selected position in the SRAM (OLED)
@param [in] line The row to go to
@param [in] column The column to go to
*/
void oled_ramgotopos(int line, int column);

/**
Writes the contents of SRAM to the OLED
*/
void oled_ramtransfer();

/**
Creates some animation
*/
void animate(unsigned int pos, unsigned int shooting);



#endif /* OLED_H_ */