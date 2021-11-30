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
		{
			printaddr(startaddr->tiny_start);
			write(2, ") (", 4);
		}
		if (startaddr->small_start == NULL)
			write(2, "NULL)\n", 6);
		else
		{
			printaddr(startaddr->small_start);
			write(2, ")\n", 2);
		}
	}
}


void	*updatestartaddr(char c)
{
	if (startaddr == NULL)
	{
		if (c == 't')
		{
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
		void *start = NULL;
		if ((start = mmap(NULL, TINY_ZONE_SIZE, PROT_READ
						| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
						| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);

		startaddrs_t	*sa = start;
		sa->tiny_start = (void*)sa + ALIGN16(sizeof(startaddrs_t));
		sa->small_start = startaddr->small_start;
		sa->first = sa->tiny_start;
		sa->next = sa;
		startaddr = sa;

		metadata	*md = sa->tiny_start;
		md->next = sa->tiny_start;
		md->size = 0;
		md->zonest = ALIGN16(sizeof(startaddrs_t));
		return (sa->tiny_start);
	}
	else if (startaddr->small_start == NULL && c == 's')
	{
		void *start = NULL;
		if ((start = mmap(NULL, SMALL_ZONE_SIZE, PROT_READ
						| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
						| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);
		startaddrs_t	*sa = start;
		sa->tiny_start = startaddr->tiny_start;
		sa->small_start = (void*)sa + ALIGN16(sizeof(startaddrs_t));
		sa->first = sa->small_start;
		sa->next = sa;
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

static void	*create_new_zone(int size, void *heap_start, metadata *prev)
{
	void	*start = NULL;
	size_t	zonesize = (heap_start == startaddr->tiny_start)
			? TINY_ZONE_SIZE : SMALL_ZONE_SIZE;
	
	if ((start = mmap(NULL, zonesize, PROT_READ
					| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
					| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	startaddrs_t *sa = start;
	sa->tiny_start = startaddr->tiny_start;
	sa->small_start = startaddr->small_start; // don't really need both of these but eh
	sa->first = (void*)start + ALIGN16(sizeof(startaddrs_t));
	startaddrs_t *saprev = (void*)prev - prev->zonest;
	sa->next = saprev->next;
	
	
	metadata *cur = (void*)start + ALIGN16(sizeof(startaddrs_t));
	cur->next = prev->next;
	cur->size = size;
	cur->zonest = ALIGN16(sizeof(startaddrs_t));
	prev->next = sa->first;
	saprev->next = sa;
	
	return (cur);
}

static void	*placeinmemory(long int size, void *heap_start) 
{
	startaddrs_t	*sa = heap_start - ALIGN16(sizeof(startaddrs_t));
	metadata	*cur = sa->first;
	metadata	*prev = NULL;
	void		*data_ptr = NULL;

	if (cur->size != 0 && cur->next != NULL // check if it's not after a zone initialization
		&& (size_t)(heap_start - (void*)sa->first)
		>= ALIGN16(sizeof(metadata)) + ALIGN16(size))
		// verifie si il y a de la place avant le debut des bloques
	{
		cur = heap_start;
		cur->size = size;		
		cur->zonest = ALIGN16(sizeof(startaddrs_t));
		cur->next = sa->first;
		sa->first = cur;
		data_ptr = (void*)cur + ALIGN16(sizeof(metadata));
		return (data_ptr);
	}
	//there's always the global vairable at the start of zones
	while (!(cur->next == heap_start
				|| ((((void*)cur->next - (void*)cur
					- ALIGN16(sizeof(metadata)) - ALIGN16(cur->size)
					>= ALIGN16(size) + ALIGN16(sizeof(metadata))))
		&& (cur->next != heap_start
			&& (void*)cur->next - cur->next->zonest == (void*)cur - cur->zonest))))
		// WTF THIS CONDITION TODO
	{
		prev = cur;
		cur = cur->next;
	}

	if (cur->next == heap_start) //  on est arrive a la fin
	{
		if ((heap_start == startaddr->tiny_start
			&& cur->zonest + ALIGN16(cur->size) + ALIGN16(sizeof(metadata))*2 + size
				>= TINY_ZONE_SIZE)
			|| (heap_start == startaddr->small_start
			&& cur->zonest + ALIGN16(cur->size) + ALIGN16(sizeof(metadata))*2 + size
				>= SMALL_ZONE_SIZE))
			// on va depasser la page memoire et il faut en appeller une autre
		{
			cur = create_new_zone(size, heap_start, cur);
		}
		else if (prev != NULL || (prev == NULL && cur->size != 0))
			// just in the middle of a zone, and end of list
		{
			// check if you're not going over a page
			prev = cur;
			cur = (void*)cur + ALIGN16(sizeof(metadata)) + ALIGN16(prev->size);
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
		}
	}
	else // on a trouve un trou de la bonne taille
	{
		prev = cur;
		cur = (void*)cur + ALIGN16(cur->size) + ALIGN16(sizeof(metadata));
		cur->size = size;
		cur->zonest = (size_t)(prev->zonest
				+ ALIGN16(sizeof(metadata)) + ALIGN16(prev->size));
		cur->next = prev->next;
		prev->next = cur;
	}

	data_ptr = (void*)cur + ALIGN16(sizeof(metadata));
	return (data_ptr);
}

void	*malloc(size_t size)
{
	void	*data_ptr = NULL;

	ft_putstr_fd("size => ", 2);
	ft_putnbr_fd(size, 2);
	ft_putstr_fd(" --- ", 2);

	if (size == 0) // TODO if they ask too much too
		return (NULL);

	if (size <= TINY)
	{
		if (startaddr == NULL || startaddr->tiny_start == NULL)
			updatestartaddr('t');
		data_ptr = placeinmemory(size, startaddr->tiny_start);
		ft_putstr_fd("TINY ADDED (", 2);
		printaddr(data_ptr);
		ft_putstr_fd(")\n", 2);
	}
	else if (size <= SMALL)
	{
		if (startaddr == NULL || startaddr->small_start == NULL)
			updatestartaddr('s');
		data_ptr = placeinmemory(size, startaddr->small_start);
		ft_putstr_fd("SMALL ADDED (", 2);
		printaddr(data_ptr);
		ft_putstr_fd(")\n", 2);
	}
	else // LARGE
	{
		if ((data_ptr = mmap(NULL, ALIGNPS(size + ALIGN16(sizeof(metadata))),
					PROT_READ | PROT_WRITE | PROT_EXEC,
					MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);
		metadata *meta = data_ptr;
		meta->size = size;
		meta->next = NULL;
		meta->zonest = 0;
		data_ptr += ALIGN16(sizeof(metadata));
		ft_putstr_fd("LARGE ADDED (", 2);
		printaddr(data_ptr);
		ft_putstr_fd(")\n", 2);
	}

	show_alloc_mem(false);

	return (data_ptr);
}
