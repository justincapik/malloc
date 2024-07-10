#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>

int main()
{
    void *test1 = calloc(sizeof(char), 128);
    test1 = realloc(test1, 1000);
    free(test1);
    
    void *test = (void*)calloc(4096, INT_MAX - 64);
    printf("test = %p\n", test);

    int a = (int)pow(2, 16), b = 2, c = a * b; // changer ici
    printf("%d * %d = %d\n", a, b, c);
    
    char *test2 = (char*)calloc(a, b);
    printf("test2 = %p\n", test2);
    test2[0] = 42;
    free(test2);
}