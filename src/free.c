#include "libft_malloc.h"

void		free(void *ptr)
{
	if (munmap(ptr, PAGE_SIZE) == -1)
		write(2, "fuck i guess\n", 13);
	else
		write(2, "ye\n", 3);
}
