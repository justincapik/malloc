#include "libft_malloc.h"

void	*realloc(void *ptr, size_t size)
{
	// TODO check ptr like free ig
	/*
	ft_putstr_fd("realloc at ", 2);
	printaddr(ptr);
	ft_putstr_fd(" of size ", 2);
	ft_putnbr_fd(size, 2);
	write(2, "\n", 1);
	*/

	metadata	*meta = ptr - ALIGN16(sizeof(metadata));
	void		*data_ptr = NULL;

	if (ptr == NULL)
	{
		data_ptr = malloc(size);
	}
	//  MAC IS DIFFERENT HERE TODO
	else if (ptr != NULL && size == 0)
	{
		free(ptr);
		data_ptr = NULL;
	}
	else if (size <= meta->size)
	{
		meta->size = size;
		data_ptr = ptr;
	}
	else
	{
		data_ptr = malloc(size);
		ft_memcpy(data_ptr, ptr, size);
		free(ptr);
	}
	
	//show_alloc_mem(false);
	return (data_ptr);
}
