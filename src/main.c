#include "mcp2515.h"
int main()
{
    char *path = "/dev/spidev0.0";
    mcp2515_dev* mcp2515_device = new_mcp2515_dev(path,3,50000);
    mcp2515_initial(mcp2515_device);
    //printf("%d",ret);
    return 0;
}
