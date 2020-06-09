#include "mcp2515.h"

mcp2515_dev* new_mcp2515_dev(char *spidev_path,uint8_t mode,uint32_t speed)
{
    mcp2515_dev* mcp2515_device = (mcp2515_dev*)malloc(sizeof(mcp2515_dev));
    mcp2515_device->spidev_path = spidev_path;
    mcp2515_device->mode = mode;
    mcp2515_device->speed = speed;
    return mcp2515_device;
}

int mcp2515_initial(mcp2515_dev *mcp2515_device)
{
    // read or set result
    int ret = 0;

    // open device
    int fd = open(mcp2515_device->spidev_path, O_RDWR); 
    if(fd < 0)return -1;

    // set CPOL => 1, CPOH => 1 
    ret = ioctl(fd, SPI_IOC_WR_MODE, 3);
    if(ret == -1)return -2;

    // get CPOL => 1, CPOH => 1 
    ret = ioctl(fd, SPI_IOC_RD_MODE, 3);
    if(ret == -1)return -2;

    //
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, 8);
    if(ret == -1)return -3;

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, 8);
    if(ret == -1)return -4;

    // speed setting
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &(mcp2515_device->speed));
    if(ret == -1)return -5;

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &(mcp2515_device->speed));
    if(ret == -1)return -5;

    return 0;
}