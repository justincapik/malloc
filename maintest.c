#include "libft_malloc.h"
#include "libft.h"

static void     write_char(char c)
{
        write(2, &c, 2);
}

static char     hex_digit(int v) {
    if (v >= 0 && v < 10)
        return '0' + v;
    else
        return 'a' + v - 10; // <-- Here
}

void            printaddr(void* p0) {
    int i;
    size_t p = (size_t)p0;

    write_char('0'); write_char('x');
    for(i = (sizeof(p) << 3) - 4; i>=0; i -= 4) {
        write_char(hex_digit((p >> i) & 0xf));
    }
}

# define COUNT 10

int 	main(void)
{
	size_t	*test[COUNT];

	for (int i = 0; i < COUNT; ++i)
	{
		test[i] = (void *)malloc(100);
	}

	for (int i = 0; i < COUNT; ++i)	
	{
		*(test[i]) = 0xffffffffffff;
	}

	for (int j = 0; j < COUNT; ++j)
	{
		for (int i = j; i >= 0; --i)	
			free(test[i]);
		for (int i = j; i >= 0; --i)	
			test[i] = (void*)malloc(100);
		for (int i = j; i < COUNT; ++i)	
			free(test[i]);
		for (int i = j; i < COUNT; ++i)	
			test[i] = (void*)malloc(100);
	}
	
	//for (int i = COUNT - 1; i >= 0; --i)	
	//	free(test[i]);

	/*
	//printf("pagesize = %d\n", PAGE_SIZE);
	int	*test1 = malloc(sizeof(int));
	int	*test2 = malloc(sizeof(int));
	int	*test3 = malloc(sizeof(int));
	int	*test4 = malloc(sizeof(int));
	
	*test1 = 1;
	*test2 = 2;
	*test3 = 3;
	*test4 = 4;
	*/

	printaddr((void*)NULL - 16);

	write(2, "pat\n", 5);
}
