#include "mcp2515.h"
int main()
{
    char *path = "/dev/spidev1.1";
    mcp2515_dev* mcp2515_device = new_mcp2515_dev(path,3,50000);
    int ret = mcp2515_initial(mcp2515_device);
    printf("%d",ret);
    return 0;
}