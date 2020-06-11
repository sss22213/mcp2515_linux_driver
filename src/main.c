#include "mcp2515.h"
int main()
{
    int ret = 0;
    char *path = "/dev/spidev0.0";
    mcp2515_dev* mcp2515_device = new_mcp2515_dev(path,Normal,500000);
    ret = mcp2515_initial(mcp2515_device);
    /*
    uint8_t data = 0x60;
    ret = mcp2515_write_register(mcp2515_device, CANCTRL, &data, 1);
    ret = mcp2515_read_register(mcp2515_device,CANSTAT,1);
    */
    mcp2515_set_mode(mcp2515_device);
    //printf("%d",ret);
    return 0;
}