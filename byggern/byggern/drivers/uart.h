/*
 * uart.h
 *
 * Created: 19.09.2013 16:23:39
 *  Author: tommp
 */ 



#ifndef UART_H_
#define UART_H_

#define BAUD_RATE_4_9MHZ 31

/**
Write a single char to uart
@param [in] val The character value
*/
void write(char val);

/**
Waits for and reads a single char received on uart interface
@retval char The received character
*/
char read_();

/**
Prints a fancy welcome message in ASCII art
*/
void printWelcomeMessage();

/**
Sets up uart and sends the welcome message
*/
void setupUartAndSendWelcomeMessage();

#endif /* UART_H_ */