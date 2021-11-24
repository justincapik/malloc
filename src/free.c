#include "libft_malloc.h"

static void	delete_zone(startaddrs_t *sa)
{
	if (sa->next == sa) // last zone (tiny or small)
	{
		if (sa->tiny_start - ALIGN16(sizeof(startaddrs_t)) == sa
				&& sa->small_start != NULL)
			// deleting tiny and small exists
		{
			startaddr = (void*)sa->small_start
				-  ALIGN16(sizeof(startaddrs_t));
			startaddr->tiny_start = NULL;
			if (munmap((void*)sa, TINY_ZONE_SIZE) == -1)
				ft_putendl("Error: munmap failed");
		}
		else if (sa->small_start - ALIGN16(sizeof(startaddrs_t)) == sa
				&& sa->tiny_start != NULL)
			// deleting small and tiny exists
		{
			startaddr = (void*)sa->tiny_start
				-  ALIGN16(sizeof(startaddrs_t));
			startaddr->small_start = NULL;
			if (munmap((void*)sa, SMALL_ZONE_SIZE) == -1)
				ft_putendl("Error: munmap failed");
		}
		else if (sa->tiny_start - ALIGN16(sizeof(startaddrs_t)) == sa)
			// deleting tiny and small doesn't exist
		{
			if (munmap((void*)sa, TINY_ZONE_SIZE) == -1)
				ft_putendl("Error: munmap failed");
		}
		else if (sa->tiny_start - ALIGN16(sizeof(startaddrs_t)) == sa)
			// deleting small and tiny doesn't exist
		{
			if (munmap((void*)sa, SMALL_ZONE_SIZE) == -1)
				ft_putendl("Error: munmap failed");
		}
		
	}
	else
	{
		
	}
}

void		free(void *ptr)
{
	ft_putstr_fd("freeing at ", 2);
	printaddr(ptr);
	ft_putstr_fd("\n", 2);

	// what about double frees
	// check if metadata makes sense for no segfaults
	// check if it's in a zone <=

	metadata *meta = (void*)ptr - ALIGN16(sizeof(metadata));

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
		if (sa->first == meta) // check if it's the first one in a zone
		{
			write(2, "YO\n", 3);
			startaddrs_t	*san = (void*)meta->next - meta->next->zonest;
			if (sa != san) // it's the last block in the zone
			{
				ft_putendl_fd("-wait");
				if (sa == startaddr->tiny_zone - ALIGN16(sizeof(startaddrs_t))
				|| sa == startaddr->small_zone - ALIGN16(sizeof(startaddrs_t)))
					delete_zone(sa);
				// link previous zone to the next one
				// check if it's deleting original zone
				// delete this zone
			}
			else // the first block in a filled or not zone
			{
				ft_putendl_fd(" da");
				sa->first = meta->next;
				meta->next = NULL;
				meta->zonest = 0;
			}
		}
		else
		{
			while (prev->next != meta)
				prev = prev->next;
			prev->next = meta->next;
			meta->next = NULL;
			meta->zonest = 0;
			write(2, ":(\n", 3);
		}

	}
	else
	{
		write(2, "=4=\n", 4);
		if (munmap((void*)meta, ALIGNPS(meta->size)) == -1)
			write(2, "fuck i guess\n", 13);
		else
			write(2, "ye\n", 3);
	}

	/*
	   if (munmap(ptr, PAGE_SIZE) == -1)
	   write(2, "fuck i guess\n", 13);
	   else
	   write(2, "freed\n", 6);
	 */
}
