# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kemartin <kemartin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/18 15:47:39 by kemartin          #+#    #+#              #
#    Updated: 2021/02/18 15:34:13 by pharatyk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft_malloc.so
SRC_DIR = ./src/
OBJ_DIR = ./obj/
SRC_NAME =	malloc.c\
		free.c

CC = gcc -g -Wall -Wextra -Werror -Wuninitialized -fPIC
CFLAGS = -I ./inc/

SRCS = $(addprefix $(SRC_DIR), $(SRC_NAME))
OBJ = $(addprefix $(OBJ_DIR), $(SRC_NAME:.c=.o))

$(NAME): $(OBJ)
	@gcc $(OBJ) -shared -o $(NAME)
	@printf " _/\nlibft  [done]\n"

all: $(NAME)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@echo $<
	@$(CC) -c $(CFLAGS) $< -o $@

.PHONY: all clean fclean re
