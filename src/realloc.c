#include "libft_malloc.h"

static bool            check_ptr(void *ptr)
{
	startaddrs_t    *sa = startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t));
	metadata *meta = sa->first;

	do
	{
		if ((void*)meta + ALIGN16(sizeof(metadata)) == ptr)
			return (true);
		meta = meta->next;
	}
	while (meta != (void*)sa + ALIGN16(sizeof(startaddrs_t)));

	sa = startaddr->small_start - ALIGN16(sizeof(startaddrs_t));
	meta = sa->first;

	do
	{
		if ((void*)meta + ALIGN16(sizeof(metadata)) == ptr)
			return (true);
		meta = meta->next;
	}
	while (meta != (void*)sa + ALIGN16(sizeof(startaddrs_t)));

	meta = startaddr->large_start;
	while (meta != NULL)
	{
		if ((void*)meta + ALIGN16(sizeof(metadata)) == ptr)
			return (true);
		meta = meta->next;
	}

	return (false);
}

void	*realloc(void *ptr, size_t size)
{
	if (DEBUG == 1)
	{
		ft_putstr_fd("Realloc: realloc at ", 2);
		printaddr(ptr);
		ft_putstr_fd(" of size ", 2);
		ft_putnbr_fd(size, 2);
		write(2, "\n", 1);
	}

	metadata	*meta = ptr - ALIGN16(sizeof(metadata));
	void		*data_ptr = NULL;

	if (ptr == NULL)
	{
		data_ptr = malloc(size);
	}
	else if (check_ptr(ptr) == false)
	{
		if (DEBUG == 1)
			ft_putstr_fd("Error: invalide pointer given to realloc\n", 2);
		return (NULL);
	}
	else if (ptr != NULL && size == 0)
	{
		free(ptr);
		data_ptr = malloc(TINY);
	}
	else if (size <= meta->size)
		/*
		   || ((void*)meta->next - (void*)meta - ALIGN16(sizeof(metadata)) < size
		   && (meta->next->zonest != ALIGN16(sizeof(startaddrs_t))
		   && meta->next->zonest != 0)))
		   */
	{
		/*
		   if ((void*)meta->next - (void*)meta - ALIGN16(sizeof(metadata)) < size
		   && (meta->next->zonest != ALIGN16(sizeof(startaddrs_t))
		   && meta->next->zonest != 0))
		   {
		   ft_putstr_fd("CAUGHT\n", 2);
		   printaddr(meta);
		   write(2, "\n", 1);
		   printaddr(meta->next);
		   write(2, "\n", 1);
		   printhex(
		   }
		   */
		meta->size = size;
		data_ptr = ptr;
	}
	else
	{
		data_ptr = malloc(size);
		ft_memcpy(data_ptr, ptr, meta->size);
		free(ptr);
	}

	return (data_ptr);
}
