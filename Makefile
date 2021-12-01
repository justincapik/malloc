#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jucapik <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/04 13:09:58 by jucapik           #+#    #+#              #
#    Updated: 2019/05/07 11:33:49 by jucapik          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME		=	libft_malloc_$(shell hostname).so

CC			=	gcc -g 


##
##		FILE DESCRIPTOR
##

INCLUDE = inc libft

SRC_PATH = src

SRCS =	global.c	\
	malloc.c	\
	free.c		\
	realloc.c	\
	calloc.c	\
	reallocarray.c	\
	debug.c


##
##		SETTING VPATH
##

vpath %.c $(foreach dir, $(SRC_PATH), $(dir):)


##
##		DEPENDENCE DESCRIPTOR
##

IDEP = inc/libft_malloc.h

OBJ_PATH = objs

OBJS = $(addprefix $(OBJ_PATH)/, $(SRCS:.c=.o))


##
##		LIB DESCRIPTOR
##

LIBFT_PATH	=	libft
LIBNAME		=	ft
LIBPATH		=	$(LIBFT_PATH)
LIBHEAD		=	$(LIBFT_PATH)


##
##		FLAGS CONSTRUCTION
##

CFLAGS += -Wall -Wextra -Werror

IFLAGS = 	$(foreach dir, $(INCLUDE), -I$(dir) ) \

LFLAGS =	$(foreach path, $(LIBPATH), -L$(path) ) \
			$(foreach lib, $(LIBNAME), -l$(lib) ) \

$(OBJ_PATH)/%.o:	%.c $(IDEP)
	$(CC) -c $< -o $@ $(CFLAGS) $(IFLAGS) -fPIC


all:		$(NAME)

$(NAME):	$(OBJ_PATH) $(OBJS)
	cd $(LIBPATH) && $(MAKE)
	$(CC) -shared -o $(NAME) $(OBJS) $(CFLAGS) $(IFLAGS) $(LFLAGS)

clean:
	make clean -C $(LIBFT_PATH)
	rm -rf $(OBJ_PATH)

fclean: clean
	make fclean -C $(LIBFT_PATH)
	rm -rf $(NAME)

$(OBJ_PATH):
	mkdir $(OBJ_PATH)

re:			fclean all

.SILENT:	all $(NAME) fclean clean re 
.PHONY:		clean fclean re
