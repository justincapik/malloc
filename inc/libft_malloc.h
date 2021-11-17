#ifndef __LIBFT_MALLOC_H__
# define __LIBFT_MALLOC_H__

#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

# define PAGE_SIZE getpagesize()

# define TINY 128 // biggest size of a single variable ?
# define SMALL (PAGE_SIZE / 8) // honestly kind of random

typedef struct metadata metadata;
struct		metadata
{
	metadata	*next; // wraps around to the start
	size_t		size;
}; 

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);

#endif
