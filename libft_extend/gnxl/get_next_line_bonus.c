/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:26:30 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/13 18:31:41 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_line_read(int fd, char *arr, char *line)
{
	int		r_bytes;
	char	*str;

	r_bytes = BUFFER_SIZE;
	while (r_bytes == BUFFER_SIZE && !(ft_strchr(line, '\n')))
	{
		r_bytes = read(fd, arr, BUFFER_SIZE);
		if (r_bytes < 0)
		{
			free(line);
			arr[0] = '\0';
			return (NULL);
		}
		arr[r_bytes] = '\0';
		str = ft_strjoin(line, arr);
		if (!str)
			return (free(line), NULL);
		free (line);
		line = str;
	}
	if (line[0] == '\0')
		return (free(line), NULL);
	return (line);
}

char	*ft_err_hand(char *arr, char *nxtl, char *line)
{
	ft_strlcpy(arr, nxtl + 1, BUFFER_SIZE + 1);
	return (ft_substr(line, 0, nxtl - line + 1));
}

char	*get_next_line(int fd)
{
	static char		matrix[1024][BUFFER_SIZE + 1];
	char			*line;
	char			*nextline;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = ft_strdup(matrix[fd]);
	if (!line)
		return (NULL);
	line = ft_line_read(fd, matrix[fd], line);
	if (!line)
		return (NULL);
	nextline = ft_strchr(line, '\n');
	if (nextline != NULL)
		return (nextline = ft_err_hand(matrix[fd], nextline, line),
			free(line), nextline);
	else
	{
		if (!ft_strlen(line))
			return (free(line), NULL);
		line[ft_strlen(line)] = '\0';
		matrix[fd][0] = '\0';
	}
	return (line);
}
