#include "libft_malloc.h"



void	*malloc(size_t size)
{
	void	*ptr = NULL;

	if ((ptr = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
			MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return (NULL);
	
	return (ptr);
}
