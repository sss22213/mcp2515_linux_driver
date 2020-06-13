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
uint8_t* spi_send(mcp2515_dev *mcp2515_device, uint8_t *data,int length)
{
    int ret = 0;

    uint8_t* tx_buffer = (uint8_t*)malloc(sizeof(uint8_t)*length);
    memcpy(tx_buffer, data, sizeof(uint8_t)*length);
    //printf("%x\n",tx_buffer[0]);

    uint8_t* rx_buffer = (uint8_t*)malloc(sizeof(uint8_t)*length);
    memset(rx_buffer,0,sizeof(uint8_t)*length);

    struct spi_ioc_transfer tr = 
    {
        .tx_buf = (unsigned long)tx_buffer,
		.rx_buf = (unsigned long)rx_buffer,
		.len = length,
		.delay_usecs = 200,
		.speed_hz = mcp2515_device->spi_dev->speed,
		.bits_per_word = mcp2515_device->spi_dev->bits,
    };

    ret = ioctl(mcp2515_device->spi_dev->fd, SPI_IOC_MESSAGE(1), &tr);
    //printf(ret);
	if (ret < 1)
    {
        printf("can't send spi message");
        exit(0);
    }
    //
    /*
    for(int idx = 0; idx < length; idx++)
        printf("%d: %x",idx,rx_buffer[idx]);
    printf("\n");
    */
    // free
    free(tx_buffer);
    // free(rx_buffer);
    return rx_buffer;
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

    printf("spi open success\n");

    return 0;
}


int mcp2515_initial(mcp2515_dev *mcp2515_device)
{
    int ret = 0;
    uint8_t data = 0;
    ret = spi_initial(mcp2515_device);
    if(ret!=0)return ret;    
    // Send RESET SPI cmd
    data = MCP2515_SPI_RESET;
    spi_send(mcp2515_device, &data, sizeof(data)/sizeof(uint8_t));
    // Clear Tx buffer
    data = 0;
    mcp2515_write_register(mcp2515_device, TXRTSCTRL, &data,sizeof(data)/sizeof(uint8_t));
    // No prescaler
    mcp2515_write_register(mcp2515_device, CANCTRL, &data,sizeof(data)/sizeof(uint8_t));
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
    uint8_t *spi_ret = spi_send(mcp2515_device,tx_buffer,data_length + 2);
    free(spi_ret);
    free(tx_buffer);
    return 0;
}

uint8_t mcp2515_read_register(mcp2515_dev *mcp2515_device, uint8_t register_address, uint8_t data_length)
{
    // tx buffer
    uint8_t result = 0;
    uint8_t *tx_buffer = (uint8_t*)malloc(sizeof(uint8_t)*(data_length + 2));
    tx_buffer[0] = MCP2515_SPI_READ;
    tx_buffer[1] = register_address;
    // rx
    uint8_t *rx_buffer = (uint8_t*)malloc(sizeof(uint8_t)*(data_length + 2));
    //
    uint8_t *spi_ret = spi_send(mcp2515_device,tx_buffer,(data_length + 2));
    free(rx_buffer);
    free(tx_buffer);
    result = spi_ret[2];
    free(spi_ret);
    return result;
}

uint8_t mcp2515_read_status(mcp2515_dev *mcp2515_device)
{
    uint8_t read_status[2] = {MCP2515_SPI_READ_STATUS,0xFF}; 
    uint8_t *spi_ret = spi_send(mcp2515_device, read_status, 2);
    uint8_t result = spi_ret[1];
    free(spi_ret);
    return result;
}

int mcp2515_modify_bit(mcp2515_dev *mcp2515_device)
{
    
} 
int mcp2515_set_mode(mcp2515_dev *mcp2515_device)
{
    int write_flag = 0;
    // write mode
    int rets = mcp2515_write_register(mcp2515_device, CANCTRL, &(mcp2515_device->mode), 1);
    if(rets!=0)
    {
        printf("mode set fail");
        return -1;
    }
    // wait write
    while(!write_flag)
    {
        sleep(1);
        uint8_t ret = mcp2515_read_register(mcp2515_device,CANSTAT,1);
        // write fail
        if(ret != mcp2515_device->mode)
        {
            printf("wait write mode");
        }
        else
        {
            write_flag = 1;
        }
    }
    return 0;
}

int mcp2515_send_data(mcp2515_dev *mcp2515_device, uint8_t* data)
{
    uint8_t *ptr_read_status = mcp2515_read_status(mcp2515_device);
    uint8_t read_status = ptr_read_status[1];

    return 0;
}