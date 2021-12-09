#include "libft_malloc.h"

void	*calloc(size_t nmemb, size_t size)
{
	if (DEBUG == 1)
	{
		ft_putstr_fd("Calloc: calloc of size ", 2);
		ft_putnbr_fd(nmemb, 2);
		ft_putstr_fd(", ", 2);
		ft_putnbr_fd(size, 2);
		ft_putstr_fd(" times", 2);
		write(2, "\n", 1);
	}
	void	*data_ptr = NULL;

	data_ptr = malloc(nmemb * size);
	if (data_ptr != NULL)
		ft_bzero(data_ptr, nmemb * size);
	return (data_ptr);
}
