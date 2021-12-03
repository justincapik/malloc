#ifndef		__LIBFT_MALLOC_H__
# define	__LIBFT_MALLOC_H__

#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include "libft.h"
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>

# define PAGE_SIZE (size_t)getpagesize()

# define ALIGN16(x) ((x + 15) & ~15) // 15 pour le 01111
# define ALIGNPS(x) ((x + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1))
	// only works if pagesize is a power of 2

# define TINY 128 // biggest size of a single variable ? NEED TO BE SMALLER THAN PAGESIZE
# define SMALL 1024 // honestly kind of random

# define TINY_ZONE_SIZE ALIGNPS(TINY * (100 + ALIGN16(sizeof(metadata))))
# define SMALL_ZONE_SIZE ALIGNPS(SMALL * (100 + ALIGN16(sizeof(metadata))))
//# define TINY_ZONE_SIZE TINY * 4
//# define SMALL_ZONE_SIZE SMALL * 4

typedef struct metadata metadata;
struct		metadata
{
	metadata	*next; // wraps around to the start
	size_t		size; // if equal to zero means it's free
			      // check size with cur->next - cur
	size_t		zonest;
}; 

typedef struct startaddrs_s startaddrs_t;
struct		startaddrs_s
{
	void		*tiny_start;
	void		*small_start;
	void		*large_start;
	startaddrs_t	*next;
	metadata	*first;
};


void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	*calloc(size_t nmemb, size_t size);
void	*reallocarray(void *ptr, size_t nmemb, size_t size);

//void	printaddr(void *p0); // TODO keep inly in show_alloc_mem() later
//void	printhex(size_t nbr); // TODO 
void	show_alloc_mem(bool showmem);

extern	startaddrs_t	*startaddr;
// can and will be pointing to any zone, but it will be the right values

#endif
