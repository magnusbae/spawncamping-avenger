#include "MCP2515.h"
#include "SPI.h"
#include <avr/io.h>

char mcp_read(char reg){
	SPI_REGISTER |= (1<<SS);
	SPI_REGISTER &= ~(1<<SS);
	
	SPI_MasterTransmit(MCP_READ);
	SPI_MasterTransmit(reg);
	char valueRead = SPI_MasterRead();
	SPI_REGISTER |= (1<<SS);
	return valueRead;
}