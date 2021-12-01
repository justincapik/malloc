#include "libft_malloc.h"

void	*reallocarray(void *ptr, size_t nmemb, size_t size)
{
	void	*data_ptr = NULL;

	data_ptr = realloc(ptr, nmemb * size);	
	return (data_ptr);
}
