#ifndef _mcp2515_h_
#define _mcp2515_h_
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

// TX Buffer Control REG
#define TXB0CTRL 0x30
#define TXB1CTRL 0x40
#define TXB2CTRL 0x50

#define TXRTSCTRL 0x0D

typedef struct mcp2515_dev mcp2515_dev;

// spi set
static uint8_t mode = 3;
static uint8_t bits = 8;

// defined mcp2515 feature
typedef struct mcp2515_dev
{
    char *spidev_path;
    uint8_t mode;
    uint32_t speed;
}mcp2515_dev;

// constructor
mcp2515_dev* new_mcp2515_dev(char *,uint8_t,uint32_t);

// open and initial mcp2515
int mcp2515_initial(mcp2515_dev*);

// mcp2515 address

#endif
