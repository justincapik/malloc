#include "libft_malloc.h"

static void	delete_zone(startaddrs_t *sa)
{
	startaddrs_t	*tmp = sa->next;
	size_t		size = 0;

	write(2, "=41=\n", 5);
	// no void* cast for tiny_start and small_start because they're already void*
	while ((tmp != startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)))
			&& tmp != startaddr->small_start - ALIGN16(sizeof(startaddrs_t)))
		tmp = tmp->next;
	size = (tmp == startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)))
		? TINY_ZONE_SIZE : SMALL_ZONE_SIZE;
	while (tmp->next != sa)
		tmp = tmp->next;
	
	metadata	*prevmeta = tmp->first;
	metadata	*meta = sa->first;
	
	if (sa != (void*)startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t))
		&& sa !=  (void*)startaddr->small_start - ALIGN16(sizeof(startaddrs_t)))
		// check if it's not the absolutle first element of the list
	{
		write(2, "=421=\n", 6);
		while (prevmeta->next != meta)
			prevmeta = prevmeta->next;
		prevmeta->next = meta->next;
	}
	else
	{
		while(prevmeta != (void*)startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t))
		&& prevmeta !=  (void*)startaddr->small_start - ALIGN16(sizeof(startaddrs_t)))
			prevmeta = prevmeta->next;
		prevmeta->next = (void*)sa->next;
	 } // TODO absolutely not sure about this else here, this whole condition actually
	
	
	write(2, "=42=\n", 5);
	if (sa->next == sa)
		// we've been fooled it's been one zone all along
	{
	write(2, "=43=\n", 5);
		if (sa == startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)))
		{
	write(2, "=44=\n", 5);
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
	write(2, "=45=\n", 5);
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
	else if (sa == (void*)startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)))
		// the zone is the start of tiny
	{
		write(2, "=461=\n", 6);
		printaddr(sa->next);
		write(2, "\n", 1);
		sa->next->tiny_start = (void*)sa->next + ALIGN16(sizeof(startaddrs_t));
		sa->next->small_start = startaddr->small_start;
		startaddr = sa->next;
		printaddr(startaddr);
		write(2, "\n", 1);
		printaddr(startaddr->tiny_start);
		write(2, "\n", 1);
	}
	else if (sa ==  (void*)startaddr->small_start - ALIGN16(sizeof(startaddrs_t)))
		// the zone is the start of small
	{
		write(2, "=462=\n", 6);
		sa->next->tiny_start = startaddr->tiny_start;
		sa->next->small_start = (void*)sa->next + ALIGN16(sizeof(startaddrs_t));
		startaddr = sa->next;
	}
	write(2, "=47=\n", 5);
	tmp->next = sa->next;
	write(2, "=48=\n", 5);
	if (munmap((void*)sa, size) == -1)
		ft_putendl_fd("Error: munmap failed", 2);
}

void		free(void *ptr)
{
	ft_putstr_fd("freeing at ", 2);
	printaddr(ptr);
	ft_putstr_fd("\n", 2);

	// check if metadata makes sense for no segfaults
	// check if it's in a zone we have access to
	// check if it's small or tiny and startaddr exists

	metadata *meta = (void*)ptr - ALIGN16(sizeof(metadata));

	if (meta->size == 0 || meta->next == NULL)
	{
		ft_putstr_fd("Warning: double free\n", 2);
		return ;
	}

	write(2, "-1-\n", 4);
	if (meta->size <= SMALL) // for small and tiny
	{
		// ALSO FREE PAGE EVEN if we're not on the first one
		// check if it's a zone we have at all

		startaddrs_t	*sa = (void*)meta - meta->zonest;
		metadata *prev = sa->first;
		// todo check if start of a zone and check if last of a zone
		write(2, "=2=\n", 4);
		
		printaddr(meta);
		write(2, "\n", 1);
		printhex(meta->zonest);
		write(2, "\n", 1);
		printaddr(sa);
		write(2, "\n", 1);
		printaddr(prev);
		write(2, "\n", 1);
		
		if (sa->first == meta) // check if it's the first one in a zone
		{
			write(2, "=3=\n", 4);
			write(2, "YO\n", 3);
			startaddrs_t	*san = (void*)meta->next - meta->next->zonest;
			printaddr(san);
			write(2, "\n", 1);	
			printaddr(sa);
			write(2, "\n", 1);	
			if (sa != san
			|| (void*)meta->next == (void*)sa->next + ALIGN16(sizeof(startaddrs_t)))
				// it's the last block in the zone
			{
				write(2, "=4=\n", 4);
				// don't care about the block
				// just delete the zone and link it back
				delete_zone(sa);
			}
			else // the first block in a filled or partly filled zone
			{
				write(2, "=5=\n", 4);
				ft_putendl_fd(" da", 2);
				sa->first = meta->next;
				meta->next = NULL;
				meta->zonest = 0;
			}
		}
		else if (meta->next == startaddr->tiny_start
			|| meta->next == startaddr->small_start)
		{
			write(2, "=6=\n", 4);
			while (prev->next != meta)
				prev = prev->next;
			prev->next = meta->next;
			printaddr(sa->first);
			write(2, "\n", 1);
			printaddr(meta);
			write(2, "\n", 1);
			printaddr(prev->next);
			write(2, "\n", 1);
			printaddr(startaddr->tiny_start);
			write(2, "\n", 1);
			meta->next = NULL;
			meta->zonest = 0;
		}
		else
		{
			write(2, "=7=\n", 4);
			while (prev->next != meta)
				prev = prev->next;
			prev->next = meta->next;
			meta->next = NULL;
			meta->zonest = 0;
		}

	}
	else
	{
		write(2, "=8=\n", 4);
		if (munmap((void*)meta, ALIGNPS(meta->size)) == -1)
			ft_putendl_fd("Error: munmap failed", 2);
		else
			write(2, "ye\n", 3);
	}

	/*
	   if (munmap(ptr, PAGE_SIZE) == -1)
	   write(2, "fuck i guess\n", 13);
	   else
	   write(2, "freed\n", 6);
	 */
	printaddr(startaddr->tiny_start);
		write(2, "\n", 1);
	print_mem(false);

}
