#include <stdlib.h>
#include <unistd.h>

int main()
{
    int i;
    char *addr;

    i = 0;
    while (i < 1024)
    {
        addr = (char*)malloc(1024);
        addr[0] = 42;
        write(1, addr, 1);
        free(addr);
        ++i;
    }
    return (0);
}