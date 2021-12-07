#include "libft_malloc.h"

static void	write_char(char c)
{
	write(2, &c, 1);
}

static char	hex_digit(int v) {
	if (v >= 0 && v < 10)
		return '0' + v;
	else
		return 'a' + v - 10; // <-- Here
}

void		printaddr(void* p0) {
	int i;
	size_t p = (size_t)p0;

	write_char('0'); write_char('x');
	for(i = (sizeof(p) << 3) - 4; i>=0; i -= 4) {
		write_char(hex_digit((p >> i) & 0xf));
	}
}

static size_t	check_base(char *base)
{
	size_t    i;
	size_t    z;

	i = 0;
	z = 0;
	if (base[0] == '\0' || base[1] == '\0')
		return (0);
	while (base[i])
	{
		z = i + 1;
		if (base[i] == '+' || base[i] == '-')
			return (0);
		if (base[i] < 32 || base[i] > 126)
			return (0);
		while (base[z])
		{
			if (base[i] == base[z])
				return (0);
			z++;
		}
		i++;
	}
	return (1);
}

static void	ft_putnbr_base(size_t nbr, char *base)
{
	size_t    size_base;
	size_t    nbr_final[100];
	int	    i;

	i = 0;
	size_base = 0;
	if (check_base(base))
	{
		while (base[size_base])
			size_base++;
		while (nbr)
		{
			nbr_final[i] = nbr % size_base;
			nbr = nbr / size_base;
			i++;
		}
		while (--i >= 0)
			ft_putchar_fd(base[nbr_final[i]], 2);
	}
}

void		printhex(void* nbr)
{
	size_t	nb = (size_t)nbr;
	ft_putstr_fd("0x", 2);
	ft_putnbr_base(nb & 0xffffffff, "0123456789abcdef");
}

static void		printhex_no0x(void* nbr)
{
	size_t	nb = (size_t)nbr;
	ft_putnbr_base(nb & 0xffffffff, "0123456789abcdef");
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
			if (meta->size >= 100)
				ft_putstr_fd("\t", fd);
			else	
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
			if (meta->size >= 100)
				ft_putstr_fd("\t", fd);
			else	
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

void		show_alloc_mem_dump()
{
	print_mem(true);
}
