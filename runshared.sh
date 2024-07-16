#!/bin/sh
#compile with:
# gcc -L$(pwd) mains/test5.c -lft_malloc_x86_64_Linux
export LD_LIBRARY_PATH=$(pwd)/libft_malloc.so:$LD_LIBRARY_PATH
$@