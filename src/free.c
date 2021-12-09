#include "libft_malloc.h"

static void		delete_zone(startaddrs_t *sa, metadata *cur)
{
	startaddrs_t	*tmp = sa->next;
	size_t		size = 0;

	// no void* cast for tiny_start and small_start because they're already void*
	while ((tmp != startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)))
			&& tmp != startaddr->small_start - ALIGN16(sizeof(startaddrs_t)))
		tmp = tmp->next;
	size = (tmp == startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)))
		? TINY_ZONE_SIZE : SMALL_ZONE_SIZE;
	while (tmp->next != sa)
		tmp = tmp->next;

	if (sa->next == sa)
		// we've been fooled it's been one zone all along
	{
			sa->first = (void*)sa + ALIGN16(sizeof(startaddrs_t));
		
			metadata	*md = (void*)sa + ALIGN16(sizeof(startaddrs_t));
			md->next = (void*)sa + ALIGN16(sizeof(startaddrs_t));
			md->size = 0;
			md->zonest = ALIGN16(sizeof(startaddrs_t));
	}
	else // multiple zones
	{
		// update metadata that points to zone that's going to be deleted
		metadata	*prev = tmp->first;
		while (prev->next != (void*)(tmp->next) + ALIGN16(sizeof(startaddrs_t))
			&& prev->next != cur)
			prev = prev->next;
		
		if (sa == (void*)startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)))
		// the zone is the start of tiny
		{
			sa->next->tiny_start = (void*)sa->next + ALIGN16(sizeof(startaddrs_t));
			sa->next->small_start = startaddr->small_start;
			sa->next->large_start = startaddr->large_start;
			startaddr = sa->next;
			prev->next = startaddr->tiny_start;
		}
		else if (sa ==  (void*)startaddr->small_start - ALIGN16(sizeof(startaddrs_t)))
		// the zone is the start of small
		{
			sa->next->tiny_start = startaddr->tiny_start;
			sa->next->small_start = (void*)sa->next + ALIGN16(sizeof(startaddrs_t));
			sa->next->large_start = startaddr->large_start;
			startaddr = sa->next;
			prev->next = startaddr->small_start;
		}
		else // zone in the middle of somewhere
		{
			// add zone->next update
			prev->next = prev->next->next;
		}
	
		tmp->next = sa->next;
		if (munmap((void*)sa, size) == -1 && DEBUG == 1)
			ft_putendl_fd("Error: munmap failed", 2);
	}
}

static bool		check_ptr(void *ptr)
{
	startaddrs_t	*sa = startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t));
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

void		free(void *ptr)
{
	if (DEBUG == 1)
	{
		ft_putstr_fd("Free: freeing at ", 2);
		printaddr(ptr);
		ft_putstr_fd("\n", 2);
	}
	if (ptr == NULL)
		return ;
	if (check_ptr(ptr) == false)
	{
		if (DEBUG == 1)
			ft_putstr_fd("Error: invalide pointer given to free\n", 2);
		return ;
	}
	
	metadata *meta = (void*)ptr - ALIGN16(sizeof(metadata));
	if (meta->size == 0 && meta->next == NULL) // never actually used
	{
		if (DEBUG == 1)
			ft_putstr_fd("Warning: double free\n", 2);
		return ;
	}

	if (meta->size <= SMALL) // for small and tiny
	{
		startaddrs_t	*sa = (void*)meta - meta->zonest;
		metadata	*prev = sa->first;
		if (sa->first == meta) // check if it's the first one in a zone
		{
			startaddrs_t	*san = (void*)meta->next - meta->next->zonest;
			if (sa != san
			|| (void*)meta->next == (void*)sa->next + ALIGN16(sizeof(startaddrs_t)))
				// it's the last block in the zone
			{
				// don't care about the block
				// just delete the zone and link it back
				delete_zone(sa, meta);
			}
			else // the first block in a filled or partly filled zone
			{
				startaddrs_t	*satmp = sa->next;
				metadata	*tmp = meta->next;
				
				while ((satmp != startaddr->tiny_start
						- ALIGN16(sizeof(startaddrs_t)))
					&& satmp != startaddr->small_start
						- ALIGN16(sizeof(startaddrs_t)))
					satmp = satmp->next;
				tmp = satmp->first;
				while (tmp->next != meta
					&& tmp->next != startaddr->tiny_start
					&& tmp->next != startaddr->small_start)
					tmp = tmp->next;
				
				if (meta != satmp->first)
				{
					tmp->next = meta->next;
				}
				else
				{
					tmp->next = (void*)satmp + ALIGN16(sizeof(startaddrs_t));
				}
				sa->first = meta->next;
				meta->next = NULL;
				meta->zonest = 0;
			}
		}
		else if (meta->next == startaddr->tiny_start
			|| meta->next == startaddr->small_start)
		{
			while (prev->next != meta)
				prev = prev->next;
			prev->next = meta->next;
			meta->next = NULL;
			meta->zonest = 0;
		}
		else
		{
			while (prev->next != meta)
				prev = prev->next;
			prev->next = meta->next;
			meta->next = NULL;
			meta->zonest = 0;
		}

	}
	else // LARGE
	{
		metadata *tmp = startaddr->large_start;
		while (tmp != NULL && tmp->next != meta && tmp->next != NULL && tmp != meta)
			tmp = tmp->next;
		if (tmp->next == meta)
		{
			tmp->next = meta->next;
			if (munmap((void*)meta, ALIGNPS(meta->size)) == -1 && DEBUG == 1)
				ft_putendl_fd("Error: munmap failed", 2);
		}
		else if (tmp == meta)
		{
			if (munmap((void*)meta, ALIGNPS(meta->size)) == -1 && DEBUG == 1)
				ft_putendl_fd("Error: munmap failed", 2);
			startaddr->large_start = NULL;
		}
		else if (DEBUG == 1)
			ft_putstr_fd("Error: large malloc error\n", 2);
	}

	//show_alloc_mem();
}
