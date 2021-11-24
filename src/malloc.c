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
			sa->tiny_start = (void*)start + ALIGN16(sizeof(startaddrs_t));
			sa->small_start = NULL;
			sa->next = start;
			sa->first = sa->tiny_start;
			startaddr = sa;

			metadata	*md = sa->tiny_start;
			md->next = sa->tiny_start;
			md->size = 0;
			md->zonest = ALIGN16(sizeof(startaddrs_t));
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
			sa->small_start = (void*)start + ALIGN16(sizeof(startaddrs_t));
			sa->tiny_start = NULL;
			sa->next = start;
			sa->first = sa->small_start;
			startaddr = sa;

			metadata	*md = sa->small_start;
			md->next = sa->small_start;
			md->size = 0;
			md->zonest = ALIGN16(sizeof(startaddrs_t));
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

		metadata	*md = sa->tiny_start;
		md->next = sa->tiny_start;
		md->size = 0;
		md->zonest = ALIGN16(sizeof(startaddrs_t));
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

		metadata	*md = sa->small_start;
		md->next = sa->small_start;
		md->size = 0;
		md->zonest = ALIGN16(sizeof(startaddrs_t));
		return (sa->small_start);
	}
	else
	{
		ft_putstr("invalide string for updatestartaddr\n");
		return (NULL);
	}
}

static void	*placeinmemory(long int size, void *heap_start) 
{
	write(2, "-1-\n", 4);
	startaddrs_t	*sa = heap_start - ALIGN16(sizeof(startaddrs_t));
	metadata	*cur = sa->first;
	metadata	*prev = NULL;
	//size_t		count = ALIGN16(sizeof(startaddrs_t)); TODO maybe later
	void		*data_ptr = NULL;

	write(2, "-2-\n", 4);
	
	if ((size_t)(heap_start - (void*)sa->first) >= ALIGN16(sizeof(metadata)) + ALIGN16(size))
		// verifie si il y a de la place avant le debut des bloques
	{
		cur->size = size;		
		cur->zonest = ALIGN16(sizeof(startaddrs_t));
		cur->next = sa->first;
		sa->first = cur;
		data_ptr = (void*)cur + ALIGN16(sizeof(metadata));
		return (data_ptr);
	}

	write(2, "-3-\n", 4);
	//there's always the global vairable at the start of zones
	while (!(cur->next == heap_start
		|| ((void*)cur->next - (void*)cur
				- ALIGN16(sizeof(metadata)) - ALIGN16(cur->size)
			> ALIGN16(size) + ALIGN16(sizeof(metadata)))))
	// not sure about the condition here
	{
		prev = cur;
		cur = cur->next;
	}
	printaddr((void*)((void*)cur->next - (void*)cur
				- ALIGN16(sizeof(metadata)) - ALIGN16(cur->size)));
	write(2, "\n", 1);
	printaddr((void*)((void*)cur->next - (void*)cur));
	write(2, "\n", 1);
	write(2, "-4-\n", 4);
	if (cur->next == heap_start) //  on est arrive a la fin
	{
		/*
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
			cur->zonest = count;
		}
		else */if (prev != NULL || (prev == NULL && cur->size != 0))
			// just in the middle of a zone, and end of list
		{
			// check if you're not going over a page
			write(2, "-5-\n", 4);
			prev = cur;
			cur = (void*)cur + ALIGN16(sizeof(metadata)) + ALIGN16(size);
			prev->next = cur;
			cur->next = heap_start;
			cur->size = size;
			if (prev != NULL)
				cur->zonest = prev->zonest + (void*)cur - (void*)prev;
			else
				cur->zonest = ALIGN16(sizeof(startaddrs_t))
					+ (void*)cur - (void*)prev;
		}
		else // prev est NULL, c'est le premier appel pour un tiny
		{
			write(2, "-6-\n", 4);
			cur->next = heap_start;
			cur->size = size;		
			cur->zonest = ALIGN16(sizeof(startaddrs_t));
			startaddrs_t *sa = heap_start - ALIGN16(sizeof(startaddrs_t));
			sa->first = cur;
			ft_putnbr_fd(cur->zonest, 2);
		}
	}
	else // on a trouve un trou de la bonne taille
	{
	printaddr(prev);
	write(2, "\n", 1);
	printaddr(startaddr->tiny_start);
	write(2, "\n", 1);
	printaddr(cur);
	write(2, "\n", 1);
	printaddr(cur->next);
	write(2, "\n", 1);
		write(2, "-7-\n", 4);
		cur = (void*)cur + ALIGN16(cur->size) + ALIGN16(sizeof(cur));
		cur->size = size;
		cur->zonest = (size_t)(void*)prev
			+ ALIGN16(sizeof(metadata)) + ALIGN16(prev->size);
		cur->next = prev->next;
		prev->next = cur;
	}
	write(2, "-8-\n", 4);
	printaddr(startaddr->tiny_start);
	write(2, "\n", 1);
	printaddr(cur);
	write(2, "\n", 1);
	void	*test = startaddr;
	printaddr((test));
	write(2, "\n", 1);
	printaddr((void*)((test - (void*)cur) * -1));
	write(2, "\n", 1);
	printaddr((void*)(cur->zonest));
	write(2, "\n", 1);
	printaddr((void*)(ALIGN16(sizeof(metadata))));
	write(2, "\n", 1);
	if (prev != NULL)
	{
		ft_putnbr_fd(prev->zonest, 2);
		write(2, " + ", 3);
	}
	ft_putnbr_fd(ALIGN16(cur->size), 2);
	write(2, " + ", 3);
	ft_putnbr_fd(ALIGN16(sizeof(metadata)), 2);
	write(2, " => ", 4);
	ft_putnbr_fd(cur->zonest, 2);
	write(2, "\n", 1);

	data_ptr = (void*)cur + ALIGN16(sizeof(metadata));
	printaddr(data_ptr);
	write(2, "\n", 1);
	return (data_ptr);
}

void	*malloc(size_t size)
{
	void	*data_ptr = NULL;

	ft_putstr_fd("size => ", 2);
	ft_putnbr(size);
	ft_putstr("\n");

	// do edge case if the user asks too much

	printsa();

	if (size <= TINY)
	{
		write(2, "TINY ADDED\n", 11);
		if (startaddr == NULL || startaddr->tiny_start == NULL)
			updatestartaddr('t');
		data_ptr = placeinmemory(size, startaddr->tiny_start);
	}
	else if (size <= SMALL)
	{
		write(2, "SMALL ADDED\n", 12);
		if (startaddr == NULL || startaddr->small_start == NULL)
			updatestartaddr('s');
		data_ptr = placeinmemory(size, startaddr->small_start);
	}
	else // LARGE
	{
		write(2, "LARGE ADDED\n", 5);
		if ((data_ptr = mmap(NULL, ALIGNPS(size + ALIGN16(sizeof(metadata))),
					PROT_READ | PROT_WRITE | PROT_EXEC,
					MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);
		metadata *meta = data_ptr;
		meta->size = size;
		meta->next = NULL;
		meta->zonest = 0;
		data_ptr += ALIGN16(sizeof(metadata));
	}

	/*	
	if ((data_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
					MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	*/
	return (data_ptr);
}
