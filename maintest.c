#include "libft_malloc.h"
//#include "libmaster.h"

# define COUNT 5

int 	main(void)
{
	int	*test[COUNT];

	for (int i = 0; i < COUNT; ++i)	
		test[i] = (void *)malloc(sizeof(int));

	for (int i = 0; i < COUNT; ++i)	
		*(test[i]) = 32;
	
	for (int i = 0; i < COUNT; ++i)	
		free((void*)test[i]);

	printf("pagesize = %d\n", PAGE_SIZE);
}
