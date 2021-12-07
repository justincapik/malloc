#include "libft_malloc.h"

static void	*updatestartaddr()
{
	void *tiny = NULL;
	void *small = NULL;

	if ((tiny = mmap(NULL, TINY_ZONE_SIZE, PROT_READ
					| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
					| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);
	if ((small = mmap(NULL, SMALL_ZONE_SIZE, PROT_READ
					| PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS
					| MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		return (NULL);

	// intialize tiny
	startaddr = tiny;
	startaddrs_t	*sa = tiny;

	sa->tiny_start = tiny + ALIGN16(sizeof(startaddrs_t));
	sa->small_start = small + ALIGN16(sizeof(startaddrs_t));
	sa->large_start = NULL;
	sa->next = sa;
	sa->first = sa->tiny_start;
	
	metadata        *md = sa->tiny_start;
	md->next = sa->tiny_start;
	md->size = 0;
	md->zonest = ALIGN16(sizeof(startaddrs_t));
	
	// intialize small
	sa = small;

	sa->tiny_start = tiny + ALIGN16(sizeof(startaddrs_t));
	sa->small_start = small + ALIGN16(sizeof(startaddrs_t));
	sa->large_start = NULL;
	sa->next = sa;
	sa->first = sa->small_start;
	
	md = sa->small_start;
	md->next = sa->small_start;
	md->size = 0;
	md->zonest = ALIGN16(sizeof(startaddrs_t));

	return (startaddr);
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
	sa->large_start = sa->large_start;

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

static bool	check_size(size_t size)
{
	struct rlimit lim;
	
	if (getrlimit(RLIMIT_DATA, &lim) != 0)
		return (false);
	if (size > lim.rlim_cur)
		return (false);
	if (getrlimit(RLIMIT_AS, &lim) != 0)
		return (false);
	if (size > lim.rlim_cur)
		return (false);
	return (true);
}


void	*malloc(size_t size)
{
	void	*data_ptr = NULL;

	/*
	   ft_putstr_fd("size => ", 2);
	   ft_putnbr_fd(size, 2);
	   ft_putstr_fd("\n", 2);
	*/

	if (size == 0)
		return (NULL);
	if (check_size(size) == false)
	{
		ft_putstr_fd("Error: invalide size\n", 2);
		return (NULL);
	}	
	if (startaddr == NULL)
		updatestartaddr();

	if (size <= TINY)
	{
		data_ptr = placeinmemory(size, startaddr->tiny_start);
		/*
		   ft_putstr_fd("TINY ADDED (", 2);
		   printaddr(data_ptr);
		   ft_putstr_fd(")\n", 2);
		*/
	}
	else if (size <= SMALL)
	{
		data_ptr = placeinmemory(size, startaddr->small_start);
		/*
		   ft_putstr_fd("SMALL ADDED (", 2);
		   printaddr(data_ptr);
		   ft_putstr_fd(")\n", 2);
		*/
	}
	else // LARGE
	{
		size_t memsize = (size_t)size + ALIGN16(sizeof(metadata));
		if ((data_ptr = mmap(NULL, ALIGNPS(memsize),
					PROT_READ | PROT_WRITE | PROT_EXEC,
					MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
			return (NULL);
		metadata *meta = data_ptr;
		meta->size = size;
		meta->zonest = 0;
		meta->next = NULL;
		
		if (startaddr->large_start == NULL)
			startaddr->large_start = meta;
		else
		{
			metadata *tmp = startaddr->large_start;
			while (tmp->next != NULL)
				tmp = tmp->next;
			tmp->next = meta;
		}
		
		data_ptr += ALIGN16(sizeof(metadata));
		/*
		   ft_putstr_fd("LARGE ADDED (", 2);
		   printaddr(data_ptr);
		   ft_putstr_fd(")\n", 2);
		*/
	}

	//show_alloc_mem(false);

	return (data_ptr);
}
