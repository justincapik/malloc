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
	tmp->next = sa->next;
	if (munmap((void*)sa, size) == -1)
		ft_putendl_fd("Error: munmap failed", 2);
}
