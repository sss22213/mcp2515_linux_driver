#ifndef _mcp2515_h_
#define _mcp2515_h_
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

/***** Register Map *******/
// TX Buffer Control REG
#define TXB0CTRL 0x30
#define TXB1CTRL 0x40
#define TXB2CTRL 0x50

// 
#define TXRTSCTRL 0x0D

// Standard Identifier Transmission buffer (MSByte)
#define TXB0SIDH 0x31 
#define TXB1SIDH 0x41
#define TXB2SIDH 0x51

// Standard Identifier Transmission buffer (LSByte)
#define TXB0SIDL (TXB0SIDH + 0x01)
#define TXB1SIDL (TXB1SIDH + 0x01)
#define TXB2SIDL (TXB2SIDH + 0x01)

// Extended Identifier 8 Reg Transmission buffer (MSByte)
#define TXB0EID8 0x33
#define TXB1EID8 0x43
#define TXB2EID8 0x53

// Extended identifier 0 Reg Transmission buffer (LSByte)
#define TXB0EID0 (TXB0EID8 + 0x10) 
#define TXB1EID0 (TXB1EID8 + 0x10) 
#define TXB2EID0 (TXB2EID8 + 0x10)

// Transmit buffer n data length code
#define TXB0DLC 0x35
#define TXB1DLC 0x45
#define TXB2DLC 0x55

// TRANSMIT BUFFER n DATA BYTE m REGISTER
#define TXB0Dm 0x36
#define TXB1Dm 0x46
#define TXB2Dm 0x56

// FILTER n STANDARD IDENTIFIER REGISTER HIGH
#define RXF0SIDH 0x00
#define RXF1SIDH 0x04
#define RXF2SIDH 0x08
#define RXF3SIDH 0x10
#define RXF4SIDH 0x14
#define RXF5SIDH 0x18

// FILTER n STANDARD IDENTIFIER REGISTER LOW
#define RXF0SIDL (RXF0SIDH + 0x01)
#define RXF1SIDL (RXF1SIDH + 0x01)
#define RXF2SIDL (RXF2SIDH + 0x01)
#define RXF3SIDL (RXF3SIDH + 0x01)
#define RXF4SIDL (RXF4SIDH + 0x01)
#define RXF5SIDL (RXF5SIDH + 0x01)

// FILTER n EXTENDED IDENTIFIER REGISTER HIGH
#define RXF0EID8 0x02
#define RXF1EID8 0x05
#define RXF2EID8 0x0A
#define RXF3EID8 0x12
#define RXF4EID8 0x16
#define RXF5EID8 0x1A

// FILTER n EXTENDED 1 REGISTER LOW
#define RXF0EID0 (RXF0EID8 + 0x01)
#define RXF1EID0 (RXF1EID8 + 0x01)
#define RXF2EID0 (RXF2EID8 + 0x01)
#define RXF3EID0 (RXF3EID8 + 0x01)
#define RXF4EID0 (RXF3EID8 + 0x01)
#define RXF5EID0 (RXF4EID8 + 0x01)

// MASK n STANDARD IDENTIFIER REGISTER HIGH
#define RXM0SIDH 0x20
#define RXM1SIDH 0x24

// MASK n STANDARD IDENTIFIER REGISTER LOW 
#define RXM0SIDL (RXM0SIDH + 0x01)
#define RXM1SIDL (RXM1SIDH + 0x01)

// MASK n EXTENDED IDENTIFIER REGISTER HIGH
#define RXM0EID8 0x22
#define RXM1EID8 0x26

// MASK n EXTENDED IDENTIFIER REGISTER LOW 
#define RXM0EID0 (RXM0EID8 + 0x01)
#define RXM1EID0 (RXM1EID8 + 0x01)

// CONFIGURATION REGISTER
#define CNF1 0x2A
#define CNF2 0x29
#define CNF3 0x28

// TRANSMIT ERROR COUNTER REGISTER
#define TEC 0x1C

// RECEIVE ERROR COUNTER REGISTER
#define REC 0x1D

// ERROR FLAG REGISTER
#define EFLG 0x2D

// CAN INTERRUPT ENABLE REGISTER
#define CANINTE 0x2B

// CAN INTERRUPT FLAG REGISTER
#define CANINTF 0x2C

// CAN CONTROL REGISTER
#define CANCTRL 0x0F

// CAN STATUS REGISTER
#define CANSTAT 0x0E

// MCP2515 SPI CMD
#define MCP2515_SPI_RESET 0x0C
#define MCP2515_SPI_READ 0x03
#define MCP2515_SPI_READ_RX_BUFFER 0x90
#define MCP2515_SPI_WRITE 0x02
#define MCP2515_SPI_LOAD_TX_BUFFER 0x40
#define MCP2515_SPI_RTS 0x80
#define MCP2515_SPI_READ_STATUS 0xA0
#define MCP2515_SPI_RX_STATUS 0xB0
#define MCP2515_SPI_BIT_MODIFY 0x05

/**************************************************/

/**************** mcp2515 mode ********************/
typedef enum{
    Normal = 0x60,
    Sleep = 0x20,
    Loopback = 0x40,
    Listen = 0x60,
    Configuration = 0x80,
}mcp2515_mode;

/*************** spi error **************************/
typedef enum
{
    Open_err = 0x00,
}spi_error;

typedef struct mcp2515_dev mcp2515_dev;
typedef struct spi_description spi_description;

// defined mcp2515 feature
typedef struct mcp2515_dev
{
    spi_description* spi_dev;
    uint8_t mode;
    uint32_t speed;
}mcp2515_dev;

// defined spi description
typedef struct spi_description
{
    char *spidev_path;
    uint8_t fd;
    uint8_t mode;
    uint8_t bits;
    uint32_t speed;
}spi_description;

// constructor
mcp2515_dev* new_mcp2515_dev(char *,uint8_t,uint32_t);

// Open and initial mcp2515
int mcp2515_initial(mcp2515_dev*);

// Send Data to MCP2515
int mcp2515_write_register(mcp2515_dev*,uint8_t,uint8_t*,int8_t);

// Receive Data from MCP2515
uint8_t mcp2515_read_register(mcp2515_dev*, uint8_t, uint8_t);

// MCP2515 mode setting
int mcp2515_set_mode(mcp2515_dev*);

// MCP2515 send data (fix 8 bytes)
int mcp2515_send_data(mcp2515_dev*, uint8_t*);

//
uint8_t mcp2515_read_status(mcp2515_dev*);

#endif