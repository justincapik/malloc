#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>

int main ()
{
    void *test1 = (void*)malloc(1024);
    void *test2 = (void*)malloc(1024);
    void *test3 = (void*)malloc(1024);
    free(test2);
    test2 = (void*)malloc(512);
}