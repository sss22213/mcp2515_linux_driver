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
int spi_send(mcp2515_dev *mcp2515_device, uint8_t *data,int length)
{
    int ret = 0;

    uint8_t* tx_buffer = (uint8_t*)malloc(sizeof(uint8_t)*length);
    memcpy(tx_buffer, data, sizeof(uint8_t)*length);

    uint8_t* rx_buffer = (uint8_t*)malloc(sizeof(uint8_t)*length);
    memset(rx_buffer,0,sizeof(uint8_t)*length);

    struct spi_ioc_transfer tr = 
    {
        .tx_buf = (unsigned long)tx_buffer,
		.rx_buf = (unsigned long)rx_buffer,
		.len = length,
		.delay_usecs = 20,
		.speed_hz = mcp2515_device->spi_dev->speed,
		.bits_per_word = mcp2515_device->spi_dev->bits,
    };

    ret = ioctl(mcp2515_device->spi_dev->fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
    	printf("can't send spi message");
        return -1;

    return 0;
}

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

    // Send RESET SPI cmd
    uint8_t data = MCP2515_SPI_RESET;
    spi_send(mcp2515_device, &data, sizeof(data)/sizeof(uint8_t));

    return 0;
}

int mcp2515_write_register(mcp2515_dev *mcp2515_device, uint8_t register_address, uint8_t *data, int8_t data_length)
{
    // tx_buffer length = write_cmd + register_address + data_length
    uint8_t *tx_buffer = (uint8_t*)malloc(sizeof(uint8_t) * (data_length + 2));
    tx_buffer[0] = MCP2515_SPI_WRITE;
    tx_buffer[1] = register_address;
    memcpy(tx_buffer + 2, data, sizeof(uint8_t) * data_length);
    // send
    return spi_send(mcp2515_device,tx_buffer,data_length);
}

int mcp2515_read_register(mcp2515_dev *mcp2515_device, uint8_t register_address, uint8_t *data, int8_t data_length)
{
    /*
    // tx_buffer length = write_cmd + register_address + data_length
    uint8_t *tx_buffer = (uint8_t*)malloc(sizeof(uint8_t) * (data_length + 2));
    tx_buffer[0] = MCP2515_SPI_WRITE;
    tx_buffer[1] = register_address;
    memcpy(tx_buffer + 2, data, data_length * sizeof(uint8_t));
    // send
    return spi_send(mcp2515_device,tx_buffer,data_length);
    */
    return 0;
}