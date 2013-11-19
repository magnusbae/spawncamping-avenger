/*
 * SPI.h
 *
 * Created: 10.10.2013 15:24:54
 *  Author: tommp
 */ 


#ifndef SPI_H_
#define SPI_H_


/**
Initalizes the SPI-port on the Atmel 162 MCU
*/
void SPI_MasterInit(void);

/**
* Transmits a byte over SPI (and receives one at the same time).
Call SPI_MasterRead after transmit to read received data.
@param [in] cData The byte to transfer.
*/
void SPI_MasterTransmit(char cData);

/**
* Reads a received byte from the SPI register
@retval char The read byte.
*/
char SPI_MasterRead();


#endif /* SPI_H_ */