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

void		printhex(size_t nbr)
{
	ft_putnbr_base(nbr, "0123456789abcdef");
}
