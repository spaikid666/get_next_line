/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalguer <asalguer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 12:13:53 by asalguer          #+#    #+#             */
/*   Updated: 2024/12/20 12:56:47 by asalguer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

ssize_t	ft_read(int fd, char **saved)
{
	ssize_t	bytes_readed;
	char	*buf;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (-1);
	bytes_readed = read(fd, buf, BUFFER_SIZE);
	if (bytes_readed <= 0)
	{
		free(buf);
		return (bytes_readed);
	}
	buf[bytes_readed] = '\0';
	*saved = ft_strdup(buf);
	free (buf);
	if (!*saved)
		return (-1);
	return (bytes_readed);
}

ssize_t	ft_keep_reading(int fd, char **saved)
{
	char	*temp;
	char	*join;
	ssize_t	bytes_readed;

	temp = ft_strdup(*saved);
	free (*saved);
	*saved = NULL;
	bytes_readed = ft_read(fd, saved);
	if (bytes_readed <= 0)
	{
		*saved = temp;
		return (bytes_readed);
	}
	join = ft_strjoin(temp, *saved);
	free (temp);
	free (*saved);
	*saved = join;
	if (!*saved)
		return (-1);
	return (bytes_readed);
}

char	*ft_split_saved(char **saved)
{
	char	*next_n;
	char	*line_return;
	char	*temp;

	next_n = ft_strchr_n(*saved);
	line_return = ft_substr(*saved, 0, next_n - *saved + 1);
	if (*next_n && *(next_n + 1))
	{
		temp = ft_strdup(next_n + 1);
		free (*saved);
		*saved = temp;
	}
	else
	{
		free (*saved);
		*saved = NULL;
	}
	return (line_return);
}

char	*get_return_line(char **saved, ssize_t bytes_readed)
{
	char	*line_return;

	if (bytes_readed < 0)
	{
		free (*saved);
		*saved = NULL;
		return (NULL);
	}
	if (ft_strchr_n(*saved) != NULL)
		line_return = ft_split_saved(saved);
	else
	{
		line_return = ft_strdup(*saved);
		free (*saved);
		*saved = NULL;
	}
	if (!line_return && *saved)
	{
		free (*saved);
		*saved = NULL;
	}
	return (line_return);
}

char	*get_next_line(int fd)
{
	static char	*saved[MAX_FD];
	char		*line_return;
	ssize_t		bytes_readed;

	bytes_readed = 0;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!saved[fd])
		bytes_readed = ft_read(fd, &saved[fd]);
	while (saved[fd] != NULL && ft_strchr_n(saved[fd]) == NULL)
	{
		bytes_readed = ft_keep_reading(fd, &saved[fd]);
		if (bytes_readed <= 0)
			break ;
	}
	line_return = get_return_line(&saved[fd], bytes_readed);
	return (line_return);
}
