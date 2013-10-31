/*
 * uart.h
 *
 * Created: 19.09.2013 16:23:39
 *  Author: tommp
 */ 



#ifndef UART_H_
#define UART_H_

#define BAUD_RATE_4_9MHZ 103

void write(char val);
char read_();
void printWelcomeMessage();
void setupUartAndSendWelcomeMessage();

#endif /* UART_H_ */