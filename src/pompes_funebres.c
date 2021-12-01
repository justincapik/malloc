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
