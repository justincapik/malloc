/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jucapik <jucapik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:33:54 by jucapik           #+#    #+#             */
/*   Updated: 2019/03/21 15:24:14 by jucapik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "libft.h"

static void	init(char **line, int *check, int *readret)
{
	*line = NULL;
	*check = BUFF_SIZE;
	*readret = -2;
}

static int	bufftoline(char buff[BUFF_SIZE], char **line)
{
	char	*tmp;
	int		i;

	i = 0;
	while (buff[i] && buff[i] != '\n')
		i++;
	if (*line == NULL)
	{
		if ((*line = ft_strndup(buff, i)) == NULL)
			return (-1);
	}
	else
	{
		if ((tmp = ft_strnjoin(*line, buff, ft_strlen(*line) + i)) == NULL)
			return (-1);
		ft_memdel((void **)line);
		*line = tmp;
	}
	return (i);
}

int			get_next_line(int fd, char **line)
{
	static char	buff[BUFF_SIZE] = "\0";
	int			i;
	int			readret;
	int			check;

	if (fd < 0 || line == NULL || BUFF_SIZE <= 0)
		return (-1);
	init(line, &check, &readret);
	while (check == BUFF_SIZE)
	{
		if (buff[0] == '\0')
		{
			if ((readret = read(fd, buff, BUFF_SIZE)) == -1)
				return (-1);
			if (readret == 0 && *line == NULL)
				return (0);
		}
		if ((i = bufftoline(buff, line)) == -1)
			return (-1);
		check = (buff[i] != '\n' && readret != 0) ? BUFF_SIZE : i;
		ft_memmove(buff, buff + i + 1, BUFF_SIZE - i);
		ft_memset(buff + BUFF_SIZE - i, '\0', i);
	}
	return (1);
}
