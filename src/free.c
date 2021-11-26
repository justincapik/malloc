#include "libft_malloc.h"

static void	delete_zone(startaddrs_t *sa)
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
	else if (sa == startaddr /*&& sa->next != sa*/)
		// the zone is the start of tiny or small
	{
		sa->next->tiny_start = startaddr->tiny_start;
		sa->next->small_start = startaddr->small_start;
		startaddr = sa->next;
		// there might be bugs from how the startaddr variable is used?
		// idk just make sure when you're creating zones
		// it's not done yet right now idk how you're going to do it
	}
	tmp->next = sa->next;
	if (munmap((void*)sa, size) == -1)
		ft_putendl_fd("Error: munmap failed", 2);
}

void		free(void *ptr)
{
	ft_putstr_fd("freeing at ", 2);
	printaddr(ptr);
	ft_putstr_fd("\n", 2);

	// what about double frees
	// 	check in the metadata if size is not already set to zero or next == NULL
	// check if metadata makes sense for no segfaults
	// check if it's in a zone we have access to
	// check if it's small or tiny and startaddr exists

	metadata *meta = (void*)ptr - ALIGN16(sizeof(metadata));

	write(2, "-1-\n", 4);
	if (meta->size <= SMALL) // for small and tiny
	{
		// ALSO WHAT IF FIRST ZONE IS EMPTY, move global to somewhere ig?
		// ALSO WTF HAPPENS IF I FREE FIRST ELEMENT I DON'T WANT TO SKIP IT
		// ALSO FREE PAGE EVEN if we're not on the first one
		printaddr(meta);
		write(2, "\n", 1);

		startaddrs_t	*sa = (void*)meta - meta->zonest;
		metadata *prev = sa->first;
		// todo check if start of a zone and check if last of a zone
		write(2, "=2=\n", 4);
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
			|| (void*)meta->next == (void*)sa + ALIGN16(sizeof(startaddrs_t)))
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
	print_mem();

}
