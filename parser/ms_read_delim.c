/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_read_delim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:12:54 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/21 15:45:25 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	get_delim(t_ms_token *token, char *delimiter)
{
	size_t	length;

	length = 0;
	while (token != NULL && token -> index >= MS_TOKEN_WILDCARD)
	{
		if (delimiter != NULL)
		{
			if (token -> index == MS_TOKEN_WILDCARD)
				delimiter[length] = '*';
			if (token -> index == MS_TOKEN_VARIABLE)
				delimiter[length] = '$';
			if (token -> index != MS_TOKEN_WILDCARD)
				ft_strlcpy(delimiter + length
					+ (token -> index == MS_TOKEN_VARIABLE), token -> content,
					ft_strlen(token -> content) + 1);
		}
		length += (token -> index == MS_TOKEN_WILDCARD);
		if (token -> index != MS_TOKEN_WILDCARD)
			length += ft_strlen(token -> content)
				+ (token -> index == MS_TOKEN_VARIABLE);
		if (!token -> concatenate_content)
			break ;
		token = token -> next;
	}
	return (length + 1);
}

t_ms_token	*get_last_delim(t_ms_token *token)
{
	t_ms_token	*current;

	current = NULL;
	while (token != NULL)
	{
		if (token -> index == MS_TOKEN_DELIM)
			current = token;
		token = token -> next;
	}
	return (current);
}

void	populate_delim(int descriptor, t_ms_token *token)
{
	char		*line;
	char *const	delimiter = ft_calloc(get_delim(token, NULL) + 1, sizeof(char));

	get_delim(token, delimiter);
	while (true)
	{
		line = readline(NULL);
		if (line == NULL || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
			return ;
		if (descriptor != -1)
			write(descriptor, line, ft_strlen(line));
		free(line);
	}
}

int	ms_read_delims(t_ms_token *token, t_ms_token *last)
{
	const int			descriptor = gc_add_fd(open("minishell_delim_file.tmp", O_CREAT | O_TRUNC | O_WRONLY | O_RDONLY, 0777));
	t_ms_token *const	last_delim = get_last_delim(token);

	//printf("ms_read_delims %d\n", descriptor);
	if (last_delim == NULL)
		return (-1);
	while (token != last)
	{
		if (token -> index == MS_TOKEN_DELIM)
	 	{
			if (token == last_delim)
				populate_delim(descriptor, token -> next);
			else
				populate_delim(-1, token -> next);
		}
		token = token -> next;
	}
	return (descriptor);
}

void	ms_traverse_delims(t_ms_sequence *sequence)
{
	size_t	object_index;

	object_index = 0;
	while (object_index < sequence -> object_count)
	{
		if (sequence -> is_sequence[object_index / 8] & (1u << (object_index % 8)))
			ms_traverse_delims(sequence -> objects[object_index]);
		else
			sequence -> delim_descriptors[object_index] = ms_read_delims(
				sequence -> objects[object_index], NULL);
		object_index++;
	}
}