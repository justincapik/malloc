#include "libft_malloc.h"

void	*malloc_memory_start = NULL;

void	*create_data_space()
{
	void	*start = NULL;

	if ((start = mmap(NULL,PAGE_SIZE * 2, PROT_READ | PROT_WRITE | PROT_EXEC,
					MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	metadata tfirst = start;
	tfirst->next = start; // wrap back
	tfirst->size = 0;
	metadata sfirst = start + PAGE_SIZE;
	sfirst->next = start + PAGE_SIZE; // wrap back
	sfirst->size = 0;
	
	return (start);
}

void	*malloc(size_t size)
{
	void	*data_ptr = NULL;

	if (malloc_memory_start == NULL)
		malloc_memory_start = create_data_space();

	if (size <= TINY)
	{
		void	*tiny_start = melloc_memory_start;
		
		metadat *cur = tiny_start;
		metadat *prev = NULL;
		int count = 0;
		while (cur->size != 0 && cur->next != tiny_start)
		{
			count = count + TINY + ALIGN16(sizeof(metadata));
			if (count >= PAGE_SIZE)
				count = 0;
			cur = cur->next;
			prev = cur;
		}
		if (cur->next == tiny_start) //  on est arrive a la fin
		{
			if (count == 0 && prev != NULL)
			// on va depasser la page memoire et il faut en appeller une autre
			{
				if ((start = mmap(NULL,PAGE_SIZE * 2, PROT_READ
					| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
					| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
					return (NULL);
				cur->next = start;
				cur = start;
				cur->next = tiny_start;
				cur->size = size;
			}
			else if (prev != NULL)
			{
				prev = cur;
				cur = sizeof(cur) + TINY;
				prev->next = cur;
				cur->next = tiny_start;
				cur->size = size;
			}
			else // prev est NULL, c'est le premier appel pour un tiny
			{
				
			}
		}
		else // on a trouve un trou
			cur->size = size;
		data_ptr = cur + ALIGN16(sizeof(cur));
		return (data_ptr);

	}
	else if (size <= SMALL)
	{
		write(2, "NOPE\n", 5);
		return NULL;
	}
	else if (size > 0) // LARGE
	{
		write(2, "NOPE\n", 5);
		return NULL;
	}
	else
	{
		write(2, "big NOPE\n", 9);
		return NULL;
	}

	/*
	   if ((data_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
	   MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	   return (NULL);
	   return (data_ptr);
	 */
	return (data_ptr);
}
