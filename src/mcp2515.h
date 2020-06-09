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

typedef struct mcp2515_dev mcp2515_dev;

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

// error handler
#endif