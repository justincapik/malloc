#!/bin/sh
export DYLD_LIBRARY_PATH=.
export DYLD_INSERT_LIBRARIES=libft_malloc_e1r7p5.42.fr.so
export DYLD_FORCE_FLAT_NAMESPACE=1
$@
