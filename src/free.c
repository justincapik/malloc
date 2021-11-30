#include "libft_malloc.h"

static void	delete_zone(startaddrs_t *sa, metadata *cur)
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

	write(2, "=211=\n", 6);
	
	if (sa->next == sa)
		// we've been fooled it's been one zone all along
	{
	write(2, "=212=\n", 6);
	
		if (sa == startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)))
		{
			if (startaddr->small_start != NULL)
			{
				startaddr = startaddr->small_start
					- ALIGN16(sizeof(startaddrs_t));
				startaddr->tiny_start = NULL;
			}
			else
				startaddr = NULL;
		}
		else if (sa == startaddr->small_start - ALIGN16(sizeof(startaddrs_t)))
		{
			if (startaddr->tiny_start != NULL)
			{
				startaddr = startaddr->tiny_start
					- ALIGN16(sizeof(startaddrs_t));
				startaddr->small_start = NULL;
			}
			else
				startaddr = NULL;
		}
	}
	else // multiple zones
	{
	write(2, "=213=\n", 6);
	
		// update metadata that points to zone that's going to be deleted
		metadata	*prev = tmp->first;
		while (prev->next != (void*)(tmp->next) + ALIGN16(sizeof(startaddrs_t))
			&& prev->next != cur)
			prev = prev->next;
	write(2, "=214=\n", 6);
	
		
		if (sa == (void*)startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)))
		// the zone is the start of tiny
		{
			sa->next->tiny_start = (void*)sa->next + ALIGN16(sizeof(startaddrs_t));
			sa->next->small_start = startaddr->small_start;
			startaddr = sa->next;
			prev->next = startaddr->tiny_start;
		}
		else if (sa ==  (void*)startaddr->small_start - ALIGN16(sizeof(startaddrs_t)))
		// the zone is the start of small
		{
			sa->next->tiny_start = startaddr->tiny_start;
			sa->next->small_start = (void*)sa->next + ALIGN16(sizeof(startaddrs_t));
			startaddr = sa->next;
			prev->next = startaddr->small_start;
		}
		else
		{
			// add zone->next update
			prev->next = prev->next->next;
		}
	
	}
	write(2, "=215=\n", 6);
	
	tmp->next = sa->next;
	if (munmap((void*)sa, size) == -1)
		ft_putendl_fd("Error: munmap failed", 2);
}

void		free(void *ptr)
{
	ft_putstr_fd("freeing at ", 2);
	printaddr(ptr);
	ft_putstr_fd("\n", 2);

	// TODO
	// check from metdatasize which zone (tiny, small, large), and then
	// if the pointer is in the zone
	// throw error or smtg if it's not
	
	metadata *meta = (void*)ptr - ALIGN16(sizeof(metadata));

	if (ptr == NULL)
		return ;
	if (meta->size == 0 || meta->next == NULL)
	{
		ft_putstr_fd("Warning: double free\n", 2);
		return ;
	}

	if (meta->size <= SMALL) // for small and tiny
	{
		// ALSO FREE PAGE EVEN if we're not on the first one
		// check if it's a zone we have at all

		write(2, "=1=\n", 4);

		startaddrs_t	*sa = (void*)meta - meta->zonest;
		metadata *prev = sa->first;
		// todo check if start of a zone and check if last of a zone
		
		if (sa->first == meta) // check if it's the first one in a zone
		{
		write(2, "=2=\n", 4);
			startaddrs_t	*san = (void*)meta->next - meta->next->zonest;
			if (sa != san
			|| (void*)meta->next == (void*)sa->next + ALIGN16(sizeof(startaddrs_t)))
				// it's the last block in the zone
			{
				// don't care about the block
				// just delete the zone and link it back
		write(2, "=21=\n", 5);
				delete_zone(sa, meta);
			}
			else // the first block in a filled or partly filled zone
			{
		write(2, "=22=\n", 5);
				startaddrs_t	*satmp = sa->next;
				metadata	*tmp = meta->next;
		write(2, "=221=\n", 6);
				while ((satmp != startaddr->tiny_start
						- ALIGN16(sizeof(startaddrs_t)))
					&& satmp != startaddr->small_start
						- ALIGN16(sizeof(startaddrs_t)))
					satmp = satmp->next;
		write(2, "=222=\n", 6);
				tmp = satmp->first;
		write(2, "=223=\n", 6);
				printaddr(startaddr->tiny_start);
				write(2, "\n", 1);
				printaddr(startaddr->small_start);
				write(2, "\n", 1);
				while (tmp->next != meta
					&& tmp->next != startaddr->tiny_start
					&& tmp->next != startaddr->small_start)
				{
					printaddr(tmp);
					write(2, " -- ", 4);
					printaddr(tmp->next);
					write(2, "\n", 1);
					tmp = tmp->next;
				}
		write(2, "=224=\n", 6);
				if (!(tmp->next == satmp->first
					&& satmp->first != startaddr->tiny_start
					&& satmp->first != startaddr->small_start))
				{
		write(2, "=2241=\n", 7);
					tmp->next = tmp->next->next;
				}
		write(2, "=225=\n", 6);
				sa->first = meta->next;
				meta->next = NULL;
				meta->zonest = 0;
			}
		}
		else if (meta->next == startaddr->tiny_start
			|| meta->next == startaddr->small_start)
		{
		write(2, "=3=\n", 4);
		
			while (prev->next != meta)
				prev = prev->next;
			prev->next = meta->next;
			printaddr(sa->first);
			meta->next = NULL;
			meta->zonest = 0;
		}
		else
		{
		write(2, "=4=\n", 4);
			while (prev->next != meta)
				prev = prev->next;
			prev->next = meta->next;
			meta->next = NULL;
			meta->zonest = 0;
		}

	}
	else
	{
		write(2, "=5=\n", 4);
		if (munmap((void*)meta, ALIGNPS(meta->size)) == -1)
			ft_putendl_fd("Error: munmap failed", 2);
		else
			write(2, "ye\n", 3);
	}
		write(2, "=6=\n", 4);

	/*
	   if (munmap(ptr, PAGE_SIZE) == -1)
	   write(2, "fuck i guess\n", 13);
	   else
	   write(2, "freed\n", 6);
	 */
	show_alloc_mem(false);

}
