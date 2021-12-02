#include "libft_malloc.h"

void	*calloc(size_t nmemb, size_t size)
{
	void	*data_ptr = NULL;

	data_ptr = malloc(nmemb * size);
	if (data_ptr != NULL)
		ft_bzero(data_ptr, nmemb * size);
	return (data_ptr);
}
