/*
 * SPI.h
 *
 * Created: 10.10.2013 15:24:54
 *  Author: tommp
 */ 


#ifndef SPI_H_
#define SPI_H_

void SPI_MasterInit(void);

void SPI_MasterTransmit(char cData);

char SPI_MasterRead();


#endif /* SPI_H_ */