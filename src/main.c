#include "mcp2515.h"
int main()
{
    int ret = 0;
    char *path = "/dev/spidev0.0";
    mcp2515_dev* mcp2515_device = new_mcp2515_dev(path,Normal,500000);
    mcp2515_initial(mcp2515_device);
    /*
    uint8_t data = 0x60;
    ret = mcp2515_write_register(mcp2515_device, CANCTRL, &data, 1);
    ret = mcp2515_read_register(mcp2515_device,CANSTAT,1);
    */
    mcp2515_set_mode(mcp2515_device);
    uint8_t data[8] = {1,2,3,4,5,6,7,5};
    for(int idx = 0; idx < 100; idx++)
    {
        //printf("Send\n");
        mcp2515_send_data(mcp2515_device, 0x15, data, 8);
        sleep(1);
    }
    
    return 0;
}