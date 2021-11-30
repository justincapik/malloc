#include "libft_malloc.h"

void	*realloc(void *ptr, size_t size)
{
	// TODO check ptr like free ig

	metadata	*meta = ptr - ALIGN16(sizeof(metadata));

	if (ptr == NULL)
		return (malloc(size));
	else if (ptr != NULL && size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (size < meta->size)
	{
		meta->size = size;
		return (ptr);
	}
	else
	{
		free(ptr);
		void *data_ptr = malloc(size);
		return (data_ptr);
	}
}
