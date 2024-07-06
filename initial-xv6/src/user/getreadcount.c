#include "../kernel/types.h"
#include "user.h"


int main(void)
{
    int readcount = getreadcount();
    printf("Total read count: %d\n", readcount);
    exit(0);            
}
