#ifndef __MCP2515_H
#define __MCP2515_H

/*
mcp2515.h

This file contains constants that are specific to the MCP2515.

Version     Date        Description
----------------------------------------------------------------------
v1.00       2003/12/11  Initial release

Copyright 2003 Kimberly Otten Software Consulting
*/

// Define MCP2515 register addresses

#define MCP_RXF0SIDH	0x00
#define MCP_RXF0SIDL	0x01
#define MCP_RXF0EID8	0x02
#define MCP_RXF0EID0	0x03
#define MCP_RXF1SIDH	0x04
#define MCP_RXF1SIDL	0x05
#define MCP_RXF1EID8	0x06
#define MCP_RXF1EID0	0x07
#define MCP_RXF2SIDH	0x08
#define MCP_RXF2SIDL	0x09
#define MCP_RXF2EID8	0x0A
#define MCP_RXF2EID0	0x0B
#define MCP_CANSTAT		0x0E
#define MCP_CANCTRL		0x0F
#define MCP_RXF3SIDH	0x10
#define MCP_RXF3SIDL	0x11
#define MCP_RXF3EID8	0x12
#define MCP_RXF3EID0	0x13
#define MCP_RXF4SIDH	0x14
#define MCP_RXF4SIDL	0x15
#define MCP_RXF4EID8	0x16
#define MCP_RXF4EID0	0x17
#define MCP_RXF5SIDH	0x18
#define MCP_RXF5SIDL	0x19
#define MCP_RXF5EID8	0x1A
#define MCP_RXF5EID0	0x1B
#define MCP_TEC			0x1C
#define MCP_REC			0x1D
#define MCP_RXM0SIDH	0x20
#define MCP_RXM0SIDL	0x21
#define MCP_RXM0EID8	0x22
#define MCP_RXM0EID0	0x23
#define MCP_RXM1SIDH	0x24
#define MCP_RXM1SIDL	0x25
#define MCP_RXM1EID8	0x26
#define MCP_RXM1EID0	0x27
#define MCP_CNF3		0x28
#define MCP_CNF2		0x29
#define MCP_CNF1		0x2A
#define MCP_CANINTE		0x2B
#define MCP_CANINTF		0x2C
#define MCP_EFLG		0x2D

#define MCP_RXB0CTRL	0x60
#define MCP_RXB0SIDH	0x61
#define MCP_RXB1CTRL	0x70
#define MCP_RXB1SIDH	0x71

#define	TXB0CTRL	0x30
#define	TXB0SIDH	0x31
#define	TXB0SIDL	0x32
#define	TXB0EID8	0x33
#define	TXB0EID0	0x34
#define	TXB0DLC	0x35
#define	TXB0D0	0x36
#define	TXB0D1	0x37
#define	TXB0D2	0x38
#define	TXB0D3	0x39
#define	TXB0D4	0x3A
#define	TXB0D5	0x3B
#define	TXB0D6	0x3C
#define	TXB0D7	0x3D

#define	TXB1CTRL	0x40
#define	TXB1SIDH	0x41
#define	TXB1SIDL	0x42
#define	TXB1EID8	0x43
#define	TXB1EID0	0x44
#define	TXB1DLC	0x45
#define	TXB1D0	0x46
#define	TXB1D1	0x47
#define	TXB1D2	0x48
#define	TXB1D3	0x49
#define	TXB1D4	0x4A
#define	TXB1D5	0x4B
#define	TXB1D6	0x4C
#define	TXB1D7	0x4D

#define	TXB2CTRL	0x50
#define	TXB2SIDH	0x51
#define	TXB2SIDL	0x52
#define	TXB2EID8	0x53
#define	TXB2EID0	0x54
#define	TXB2DLC	0x55
#define	TXB2D0	0x56
#define	TXB2D1	0x57
#define	TXB2D2	0x58
#define	TXB2D3	0x59
#define	TXB2D4	0x5A
#define	TXB2D5	0x5B
#define	TXB2D6	0x5C
#define	TXB2D7	0x5D


#define MCP_TX_INT		0x1C		// Enable all transmit interrupts
#define MCP_TX01_INT	0x0C		// Enable TXB0 and TXB1 interrupts
#define MCP_RX_INT		0x03		// Enable receive interrupts
#define MCP_NO_INT		0x00		// Disable all interrupts

#define MCP_TX01_MASK	0x14
#define MCP_TX_MASK		0x54

// Define SPI Instruction Set

#define MCP_WRITE		0x02

#define MCP_READ		0x03

#define MCP_BITMOD		0x05

#define MCP_LOAD_TX0	0x40
#define MCP_LOAD_TX1	0x42
#define MCP_LOAD_TX2	0x44

#define MCP_RTS_TX0		0x81
#define MCP_RTS_TX1		0x82
#define MCP_RTS_TX2		0x84
#define MCP_RTS_ALL		0x87

#define MCP_READ_RX0	0x90
#define MCP_READ_RX1	0x94

#define MCP_READ_STATUS	0xA0

#define MCP_RX_STATUS	0xB0

#define MCP_RESET		0xC0


// CANCTRL Register Values

#define MODE_NORMAL     0x00
#define MODE_SLEEP      0x20
#define MODE_LOOPBACK   0x40
#define MODE_LISTENONLY 0x60
#define MODE_CONFIG     0x80
#define MODE_POWERUP	0xE0
#define MODE_MASK		0xE0
#define ABORT_TX        0x10
#define MODE_ONESHOT	0x08
#define CLKOUT_ENABLE	0x04
#define CLKOUT_DISABLE	0x00
#define CLKOUT_PS1		0x00
#define CLKOUT_PS2		0x01
#define CLKOUT_PS4		0x02
#define CLKOUT_PS8		0x03


// CNF1 Register Values

#define SJW1            0x00
#define SJW2            0x40
#define SJW3            0x80
#define SJW4            0xC0


// CNF2 Register Values

#define BTLMODE			0x80
#define SAMPLE_1X       0x00
#define SAMPLE_3X       0x40


// CNF3 Register Values

#define SOF_ENABLE		0x80
#define SOF_DISABLE		0x00
#define WAKFIL_ENABLE	0x40
#define WAKFIL_DISABLE	0x00


// CANINTF Register Bits

#define MCP_RX0IF		0x01
#define MCP_RX1IF		0x02
#define MCP_TX0IF		0x04
#define MCP_TX1IF		0x08
#define MCP_TX2IF		0x10
#define MCP_ERRIF		0x20
#define MCP_WAKIF		0x40
#define MCP_MERRF		0x80


//Control pin for MCP2515
#define CHIP_SELECT PB4
#define SPI_REGISTER PORTB

void mcp_init();
char mcp_read(char reg);
void mcp_write(char reg, char byte);
void mcp_bit_modify_instruction(char reg, char mask, char data);
void mcp_reset();
char mcp_read_status();
void mcp_request_to_send(int reg);
int mcp_ready_to_send(int reg);
void mcp_clear_interrupt();

#endif
