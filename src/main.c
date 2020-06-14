#include "mcp2515.h"
int main()
{
    int ret = 0;
    char *path = "/dev/spidev0.0";
    mcp2515_dev* mcp2515_device = new_mcp2515_dev(path,Normal,500000);
    // initial spi and mcp2515
    mcp2515_initial(mcp2515_device);
    // set can speed
    mcp2515_can_speed(mcp2515_device,can_500kpbs);
    // set mcp2515 mode
    mcp2515_set_mode(mcp2515_device);
    uint8_t data[8] = {1,2,3,4,5,6,0,0};
    for(int idx = 0; idx < 1000; idx++)
    {
        data[6] = (uint8_t)(idx / 256);
        data[7] = (uint8_t)(idx % 256);
        mcp2515_send_data(mcp2515_device, 0x15, data, 8);
        sleep(1);
    }
    return 0;
}