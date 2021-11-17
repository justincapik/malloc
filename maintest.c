#include "libft_malloc.h"

int 	main(void)
{
	write(2, "before\n", 7);
	void	*test = (void *)malloc(32);
	write(2, "in-between\n", 11);
	free(test);
	write(2, "after\n", 6);
}
