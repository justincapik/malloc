#include "libft_malloc.h"

void	printsa()
{
	if (startaddr == NULL)
		write(2, "SA NULL -> () ()\n", 21);
	else
	{
		write(2, "SA SMTG -> (", 12);

		if (startaddr->tiny_start == NULL)
			write(2, "NULL) (", 7);
		else
			write(2, "SMTG) (", 7);

		if (startaddr->small_start == NULL)
			write(2, "NULL)\n", 6);
		else
			write(2, "SMTG)\n", 6);
	}
}


void	*updatestartaddr(char c)
{
	write(2, "FACK", 4);
	if (startaddr == NULL)
	{
		if (c == 't')
		{
			write(2, "(1)\n", 4);
			void	*start = NULL;
			if ((start = mmap(NULL, TINY_ZONE_SIZE, PROT_READ
							| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
							| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
				return (NULL);

			startaddrs_t	*sa = start;
			sa->tiny_start = start + sizeof(startaddrs_t);
			sa->small_start = NULL;
			startaddr = sa;

			metadata	*md = sa->tiny_start + sizeof(startaddrs_t);
			md->next = sa->tiny_start;
			md->size = 0;
			md->pagem = sizeof(startaddrs_t);
			return (sa->tiny_start);
		}
		else if (c == 's')
		{
			write(2, "(2)\n", 4);
			void	*start = NULL;
			if ((start = mmap(NULL, SMALL_ZONE_SIZE, PROT_READ
							| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
							| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
				return (NULL);

			startaddrs_t	*sa = start;
			sa->small_start = start + sizeof(startaddrs_t);
			sa->tiny_start = NULL;
			startaddr = sa;

			metadata	*md = sa->small_start + sizeof(startaddrs_t);
			md->next = sa->small_start;
			md->size = 0;
			md->pagem = sizeof(startaddrs_t);
			return (sa->small_start);
		}
		else
		{
			ft_putstr("invalide string for updatestartaddr and null startaddr\n");
			return (NULL);
		}
	}
	else if (startaddr->tiny_start == NULL && c == 't')
	{
		write(2, "(3)\n", 4);
		if ((startaddr->tiny_start = mmap(NULL, TINY_ZONE_SIZE, PROT_READ
						| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
						| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);
		startaddrs_t	*sa = startaddr->tiny_start;
		sa->tiny_start = startaddr->tiny_start;
		sa->small_start = startaddr->small_start;
		startaddr = sa;

		metadata	*md = sa->tiny_start + sizeof(startaddrs_t);
		md->next = sa->tiny_start;
		md->size = 0;
		md->pagem = sizeof(startaddrs_t);
		return (sa->tiny_start);
	}
	else if (startaddr->small_start == NULL && c == 's')
	{
		write(2, "(4)\n", 4);
		if ((startaddr->small_start = mmap(NULL, SMALL_ZONE_SIZE, PROT_READ
						| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
						| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);
		startaddrs_t	*sa = startaddr->small_start;
		sa->tiny_start = startaddr->tiny_start;
		sa->small_start = startaddr->small_start;
		startaddr = sa;

		metadata	*md = sa->small_start + sizeof(startaddrs_t);
		md->next = sa->small_start;
		md->size = 0;
		md->pagem = sizeof(startaddrs_t);
		return (sa->small_start);
	}
	else
	{
		ft_putstr("invalide string for updatestartaddr\n");
		return (NULL);
	}
}

void	*malloc(size_t size)
{
	void	*data_ptr = NULL;

	printsa();

	if (size <= TINY)
	{
		write(2, "TINY ADDED\n", 11);
		if (startaddr == NULL || startaddr->tiny_start == NULL)
			updatestartaddr('t');
	}
	else if (size <= SMALL)
	{
		write(2, "SMALL ADDED\n", 12);
		if (startaddr == NULL || startaddr->small_start == NULL)
			updatestartaddr('s');
	}
	else if (size > 0) // LARGE
	{
		write(2, "NOPE\n", 5);
		//return NULL;
	}
	else
	{
		write(2, "big NOPE\n", 9);
		//return NULL;
	}
	//startaddr = NULL;
	if ((data_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
					MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	return (data_ptr);
}

void	*placeinmemory(size_t size, void *heap_start, int datasize) 
{

	metadat *cur = heap_start;
	metadat *prev = NULL;
	int count = sizeof(startaddr); //there's always the global vairable at the start of zones
	while (!(cur->size == 0 || cur->size >= datasize) || cur->next == heap_start)
	// not sure about the condition here
	{
		count = count + ALIGN16(datasize) + ALIGN16(sizeof(metadata));
		if (count >= PAGE_SIZE)
			count = 0;
		cur = cur->next;
		prev = cur;
	}
	if (cur->next == heap_start) //  on est arrive a la fin
	{
		if (count == 0 && prev != NULL)
			// on va depasser la page memoire et il faut en appeller une autre
		{
			void	*start = NULL;
			if ((start = mmap(NULL, TINY_ZONE_SIZE, PROT_READ
							| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
							| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
				return (NULL);
			cur->next = start;
			cur = start;

			cur->next = heap_start;
			cur->size = size;
			cur->pagem = count;
		}
		else if (prev != NULL) // just in the middle of a zone, and end of list
		{
			prev = cur;
			cur = sizeof(cur) + datasize;
			prev->next = cur;
			cur->next = heap_start;
			cur->size = size;
			cur->pagem = count;
		}
		else // prev est NULL, c'est le premier appel pour un tiny
		{
			cur->next = heap_start;
			cur->size = size;		
			cur->pagem = count;
		}
	}
	else // on a trouve un trou
		cur->size = size;
	data_ptr = cur + ALIGN16(sizeof(cur));
	return (data_ptr);
}
