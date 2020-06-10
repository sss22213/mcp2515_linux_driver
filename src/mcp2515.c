#include "mcp2515.h"

mcp2515_dev* new_mcp2515_dev(char *spidev_path,uint8_t mode,uint32_t speed)
{
    mcp2515_dev* mcp2515_device = (mcp2515_dev*)malloc(sizeof(mcp2515_dev));
    mcp2515_device->spi_dev = (spi_description*)malloc(sizeof(spi_description));
    mcp2515_device->spi_dev->spidev_path = spidev_path;
    mcp2515_device->spi_dev->mode = 3;
    mcp2515_device->spi_dev->bits = 8;
    mcp2515_device->spi_dev->speed = speed;
    mcp2515_device->speed = speed;
    mcp2515_device->mode = mode;
    return mcp2515_device;
}
/*
mcp2515_dev* new_mcp2515_dev(char *spidev_path,uint8_t mode,uint32_t speed)
{
    mcp2515_dev* mcp2515_device = (mcp2515_dev*)malloc(sizeof(mcp2515_dev));
    mcp2515_device->spidev_path = spidev_path;
    mcp2515_device->mode = mode;
    mcp2515_device->speed = speed;
    return mcp2515_device;
}
*/

int spi_initial(mcp2515_dev *mcp2515_device)
{
    // read or set result
    int ret = 0;

    // open device
    int fd = open(mcp2515_device->spi_dev->spidev_path, O_RDWR); 
    if(fd < 0)return -1;
    mcp2515_device->spi_dev->fd = fd;

    // set CPOL => 1, CPOH => 1 
    ret = ioctl(fd, SPI_IOC_WR_MODE, &(mcp2515_device->spi_dev->mode));
    if(ret == -1)return -2;

    // get CPOL => 1, CPOH => 1 
    ret = ioctl(fd, SPI_IOC_RD_MODE, &(mcp2515_device->spi_dev->mode));
    if(ret == -1)return -3;

    //
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &(mcp2515_device->spi_dev->bits));
    if(ret == -1)return -4;

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &(mcp2515_device->spi_dev->bits));
    if(ret == -1)return -5;

    // speed setting
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &(mcp2515_device->spi_dev->speed));
    if(ret == -1)return -6;

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &(mcp2515_device->spi_dev->speed));
    if(ret == -1)return -7;

    return 0;
}


int mcp2515_initial(mcp2515_dev *mcp2515_device)
{
  int ret = 0;
  ret = spi_initial(mcp2515_device);
  if(ret!=0)return ret;
  return 0;
}

int mcp2515_send(mcp2515_dev *mcp2515_device,uint8_t *data,int length)
{
    
}