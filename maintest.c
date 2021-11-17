#include "libft_malloc.h"
#include "libmaster.h"

int 	main(void)
{
	write(2, "before\n", 7);
	int	*test = (void *)malloc(sizeof(int));
	write(2, "in-between\n", 11);
	*test = 32;
	free((void*)test);
	printf("printf plz\n");
	write(2, "after\n", 6);
}
