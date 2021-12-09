#include "libft_malloc.h"

int		main()
{
	int	i;
	char  *addr;

	i = 0;
	while (i < 1)
	{
		addr = (char*)malloc(100);
		addr[0] = 42;
		free(addr);
		free(addr);
		i++;
	}

	show_alloc_mem_ex();
	return (0);
}
