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

#if __APPLE__
# define PAGE_SIZE (size_t)getpagesize()
#elif __ANDROID__ || __linux__ || __unix__ || defined(_POSIX_VERSION)
# define PAGE_SIZE (size_t)sysconf(_SC_PAGESIZE)
#else
#   error "Unknown compiler"
#endif





# define DEBUG 0 // 1 if debugging, 0 if not

# define ALIGN16(x) ((x + 15) & ~15) // 15 pour le 01111
// # define ALIGN16(x) x // 15 pour le 01111
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

void	show_alloc_mem(void);
void	show_alloc_mem_ex(void);

extern	startaddrs_t	*startaddr;

#endif
