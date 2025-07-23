/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_traverse_delims.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:12:54 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/23 16:10:16 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *get_delim_string(t_ms_token *token)
{
	char		*delimiter = NULL;
	char		*append;
	char		*tmp;
	uint32_t	concatenation;

	concatenation = token -> concatenation;
	while (token != NULL && token->index >= MS_TOKEN_WILDCARD
		&& token -> concatenation == concatenation)
	{
		if (token->index == MS_TOKEN_WILDCARD)
			append = ft_strdup("*");
		else if (token->index == MS_TOKEN_VARIABLE)
		{
			tmp = ft_strjoin("$", token->content);
			append = tmp;
		}
		else
			append = ft_strdup(token->content);
		tmp = delimiter;
		if (delimiter == NULL)
			delimiter = "";
		delimiter = ft_strjoin(delimiter, append);
		free(tmp);
		free(append);
		token = token -> next;
	}
	return (delimiter);
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
	char		*delimiter;

	delimiter = get_delim_string(token);
	printf("\033[1;35m[%s]\033[0;0m\n", delimiter);
	while (true)
	{
		line = readline("\033[1;35m-> \033[0;0m");
		if (line == NULL || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			free(delimiter);
			return ;
		}
		if (descriptor != -1)
		{
			write(descriptor, line, ft_strlen(line));
			write(descriptor, "\n", 1);
		}
		free(line);
	}
}

int	ms_read_delims(t_ms_token *token, t_ms_token *last)
{
	t_ms_token *const	last_delim = get_last_delim(token);
	int					write_fd;
	int					read_fd;

	if (last_delim == NULL)
		return (-1);
	write_fd = open("ms_delim_file.tmp", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (write_fd == -1)
		return (-1);
	while (token != last)
	{
		if (token -> index == MS_TOKEN_DELIM)
		{
			if (token == last_delim)
				populate_delim(write_fd, token->next);
			else
				populate_delim(-1, token->next);
		}
		token = token->next;
	}
	close(write_fd);
	read_fd = gc_add_fd(open("ms_delim_file.tmp", O_RDONLY));
	if (read_fd == -1)
		return (-1);
	return (read_fd);
}


void	ms_traverse_delims(t_ms_sequence *sequence)
{
	size_t	obj_idx;

	obj_idx = 0;
	while (obj_idx < sequence -> object_count)
	{
		if (sequence -> is_sequence[obj_idx / 8] & (1u << (obj_idx % 8)))
			ms_traverse_delims(sequence -> objects[obj_idx]);
		else
			sequence -> delim_descriptors[obj_idx] = ms_read_delims(
					sequence -> objects[obj_idx], NULL);
		obj_idx++;
	}
}
