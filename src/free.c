#include "libft_malloc.h"

void		free(void *ptr)
{
	/*
	metadata meta = ptr - sizeof(metadata);

	if (meta->size <= TINY)
	{
		meta->size = 0;
		//		if (
	}
	else if (meta->size <= SMALL)
	{

	}
	else
	{
		if (munmap(ptr, PAGE_SIZE) == -1)
			write(2, "fuck i guess\n", 13);
		else
			write(2, "ye\n", 3);
	}
	*/
	if (munmap(ptr, PAGE_SIZE) == -1)
		write(2, "fuck i guess\n", 13);
	else
		write(2, "freed\n", 6);
}
