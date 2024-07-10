#include "libft_malloc.h"

static void		printhex(void* nbr)
{
	size_t	nb = (size_t)nbr;
	ft_putstr_fd("0x", 2);
	ft_putnbr_base_fd(nb & 0xffffffff, "0123456789abcdef", 2);
}

static void		printhex_no0x(void* nbr)
{
	size_t	nb = (size_t)nbr;
	ft_putnbr_base_fd(nb & 0xffffffff, "0123456789abcdef", 2);
}

static void		printaddr(void* p0) {
	size_t	nb = (size_t)p0;
	ft_putstr_fd("0x", 2);
	ft_putnbr_base_fd(nb, "0123456789abcdef", 2);
}

static size_t	print_zone(int fd, bool showmem, startaddrs_t *stsa)
{
	metadata	*meta = stsa->first;
	startaddrs_t	*cursa = stsa;
	size_t		totalmem = 0;

	do	
	{
		totalmem += meta->size;
		if (meta == cursa->first)
		{
			ft_putstr_fd("\tNew Zone: ", fd);
			printaddr(cursa);
			ft_putstr_fd(" - ", 2);
			if (stsa == startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)))
				printaddr((void*)cursa + TINY_ZONE_SIZE);
			else
				printaddr((void*)cursa + SMALL_ZONE_SIZE);
			ft_putstr_fd("\n", 2);
		}
		printhex((void*)meta + ALIGN16(sizeof(metadata)));
		ft_putstr_fd(" - ", fd);
		printhex((void*)meta + ALIGN16(sizeof(metadata)) + ALIGN16(meta->size));
		ft_putstr_fd(" [", fd);
		ft_putnbr_fd(meta->size, fd);
		ft_putstr_fd(" bytes]", fd);
		if (showmem == true)
		{
			ft_putstr_fd(":", fd);
			ft_putstr_fd("\t\t", fd);
			char	*mem = (void*)meta + ALIGN16(sizeof(metadata));
			for(size_t i = 0; i < meta->size; ++i)
			{
				if ((*mem & 0xff) == 0)
					ft_putstr_fd("00", fd);
				else if ((*mem & 0xff) <= 0xf)
				{
					ft_putstr_fd("0", fd);
				}
				printhex_no0x((void*)((size_t)*mem & 0xff));
				++mem;
				ft_putstr_fd(" ", fd);
				if ((i + 1) % 16 == 0 && i != meta->size - 1)
					ft_putstr_fd("\n\t\t\t\t\t\t", fd);
			}
		}
		ft_putstr_fd("\n", fd);
		meta = meta->next;
		if (meta != (void*)stsa + ALIGN16(sizeof(startaddrs_t)))
		{
			cursa = (void*)meta - meta->zonest;
		}
	} while (meta != stsa->first && meta->size != 0
		&& meta != (void*)stsa + ALIGN16(sizeof(startaddrs_t)));
	return (totalmem);
}

static size_t	print_large(int fd, bool showmem, metadata *meta)
{
	size_t totalmem = 0;

	while (meta != NULL)
	{
		totalmem += meta->size;
		printhex((void*)meta + ALIGN16(sizeof(metadata)));
		ft_putstr_fd(" - ", fd);
		printhex((void*)meta + ALIGN16(sizeof(metadata)) + ALIGN16(meta->size));
		ft_putstr_fd(" [", fd);
		ft_putnbr_fd(meta->size, fd);
		ft_putstr_fd(" bytes]", fd);
		if (showmem == true)
		{
			ft_putstr_fd(":", fd);
			ft_putstr_fd("\t\t", fd);
			char	*mem = (void*)meta + ALIGN16(sizeof(metadata));
			for(size_t i = 0; i < meta->size; ++i)
			{
				if ((*mem & 0xff) == 0)
					ft_putstr_fd("00", fd);
				else if ((*mem & 0xff) <= 0xf)
				{
					ft_putstr_fd("0", fd);
				}
				printhex_no0x((void*)((size_t)*mem & 0xff));
				++mem;
				ft_putstr_fd(" ", fd);
				if ((i + 1) % 16 == 0)
					ft_putstr_fd("\n\t\t\t\t\t\t", fd);
			}
		}
		ft_putstr_fd("\n", fd);
		meta = meta->next;
	}
	return (totalmem);
}

static void		print_mem(bool showmem)
{
	int fd = 2;
	size_t totalmem = 0;

	if (startaddr == NULL)
	{
		ft_putstr_fd(
		"\n[=========================Print Memory========================]\n", fd);
		ft_putstr_fd("No memory yet\n", fd);
		ft_putstr_fd(
		"[========================End Print Memory=====================]\n", fd);
		return;
	}
	
	ft_putstr_fd("\n[=========================Print Memory========================]\n", fd);
	
	startaddrs_t	*sa = startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t));
	metadata	*meta = sa->first;
	
	ft_putstr_fd("\t\tTiny Zone:\n", fd);
	if (meta->size != 0)
		totalmem += print_zone(2, showmem,
				(void*)startaddr->tiny_start - ALIGN16(sizeof(startaddrs_t)));
	else
		ft_putstr_fd("Empty\n", 2);
	
	sa = startaddr->small_start - ALIGN16(sizeof(startaddrs_t));
	meta = sa->first;

	ft_putstr_fd("\n\t\tSmall Zone:\n", fd);
	if (meta->size != 0)
		totalmem += print_zone(2, showmem,
				(void*)startaddr->small_start - ALIGN16(sizeof(startaddrs_t)));
	else
		ft_putstr_fd("Empty\n", 2);
	
	meta = startaddr->large_start;

	ft_putstr_fd("\n\t\tLarge Zone:\n", fd);
	if (meta != NULL)
		totalmem += print_large(2, showmem, startaddr->large_start);
	else
		ft_putstr_fd("Empty\n", 2);

	ft_putstr_fd("\n\tTotal memory allocated: ", fd);
	ft_putnbr_fd(totalmem, 2);
	ft_putstr_fd(" bytes\n", 2);
	ft_putstr_fd("[========================End Print Memory=====================]\n", fd);
}

void		show_alloc_mem()
{
	print_mem(false);
}

void		show_alloc_mem_ex()
{
	print_mem(true);
}
