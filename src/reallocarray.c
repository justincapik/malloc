#include "libft_malloc.h"

static void		printaddr(void* p0) {
	size_t	nb = (size_t)p0;
	ft_putstr_fd("0x", 2);
	ft_putnbr_base_fd(nb, "0123456789abcdef", 2);
}

void	*reallocarray(void *ptr, size_t nmemb, size_t size)
{
	if (DEBUG == 1)
	{
		ft_putstr_fd("Reallocarray: reallocarray at ", 2);
		printaddr(ptr);
		ft_putstr_fd(" of size ", 2);
		ft_putnbr_fd(nmemb, 2);
		ft_putstr_fd(", ", 2);
		ft_putnbr_fd(size, 2);
		ft_putstr_fd(" times", 2);
		write(2, "\n", 1);
	}
	void	*data_ptr = NULL;

	data_ptr = realloc(ptr, nmemb * size);	
	return (data_ptr);
}
