# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jucapik <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/12 14:23:08 by jucapik           #+#    #+#              #
#    Updated: 2019/04/25 09:45:27 by jucapik          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= libft.a

CFLAGS	+= -Wall -Wextra -Werror

SRCS	=		ft_memalloc.c ft_bzero.c ft_memmove.c ft_memcpy.c \
				ft_memccpy.c ft_memset.c ft_memchr.c ft_memchr.c \
				ft_strdup.c ft_strcpy.c ft_strncpy.c ft_strcat.c \
				ft_strncat.c ft_strchr.c ft_strrchr.c ft_strstr.c \
				ft_strnstr.c ft_strnew.c ft_strmap.c ft_strmapi.c \
				ft_strsub.c ft_strjoin.c ft_strtrim.c ft_strsplit.c \
				ft_itoa.c ft_strlcat.c ft_strlen.c ft_isalpha.c \
				ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
				ft_toupper.c ft_tolower.c ft_strcmp.c ft_strncmp.c \
				ft_memcmp.c ft_atoi.c ft_strequ.c ft_strnequ.c \
				ft_memdel.c ft_strclr.c ft_striter.c ft_striteri.c \
				ft_putchar.c ft_putstr.c ft_putendl.c ft_putnbr.c \
				ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
				ft_putnbr_fd.c ft_strdel.c ft_lstnew.c ft_lstdelone.c \
				ft_lstdel.c ft_lstadd.c ft_lstiter.c ft_lstmap.c \
				ft_strlen_s.c ft_strcpy_s.c ft_memccpy_s.c ft_memset_s.c \
				ft_lstaddend.c ft_strndup.c ft_abs.c get_next_line.c \
				ft_strnjoin.c ft_lstsize.c ft_strinsert.c ft_pow.c \
				ft_putnstr.c ft_putnbr_base_fd.c

BNRY	= $(SRCS:.c=.o)

%.o:	%.c
	gcc -o $@ -c $^ $(CFLAGS)

all: $(NAME)

$(NAME): $(BNRY)
	@ar rc $(NAME) $(BNRY)
	ranlib $(NAME)

clean:
	/bin/rm -rf $(BNRY)

fclean : clean
	/bin/rm -rf libft.h.gch
	/bin/rm -rf $(NAME)

re: fclean all
