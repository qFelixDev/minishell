/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_read_delim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:12:54 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 15:39:52 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *get_delim_string(t_ms_token *token)
{
	char	*delimiter = NULL;
	char	*append;
	char	*tmp;

	while (token && token->index >= MS_TOKEN_WILDCARD)
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
		delimiter = ft_strjoin(delimiter ? delimiter : "", append);
		free(tmp);
		free(append);

		if (!token->concatenate_content)
			break;
		token = token->next;
	}
	return (delimiter);
}


//size_t	get_delim(t_ms_token *token, char *delimiter)
//{
//	size_t	length;
//
//	length = 0;
//	while (token != NULL && token -> index >= MS_TOKEN_WILDCARD)
//	{
//		if (delimiter != NULL)
//		{
//			if (token -> index == MS_TOKEN_WILDCARD)
//				delimiter[length] = '*';
//			if (token -> index == MS_TOKEN_VARIABLE)
//				delimiter[length] = '$';
//			if (token -> index != MS_TOKEN_WILDCARD)
//				ft_strlcpy(delimiter + length
//					+ (token -> index == MS_TOKEN_VARIABLE), token -> content,
//					ft_strlen(token -> content) + 1);
//		}
//		length += (token -> index == MS_TOKEN_WILDCARD);
//		if (token -> index != MS_TOKEN_WILDCARD)
//			length += ft_strlen(token -> content)
//				+ (token -> index == MS_TOKEN_VARIABLE);
//		if (!token -> concatenate_content)
//			break ;
//		token = token -> next;
//	}
//	return (length + 1);
//}

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

//void	populate_delim(int descriptor, t_ms_token *token)
//{
//	char		*line;
//	char *const	delimiter = ft_calloc(get_delim(token, NULL) + 1, sizeof(char));
//
//	get_delim(token, delimiter);
//	while (true)
//	{
//		line = readline(NULL);
//		if (line == NULL || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
//			return ;
//		if (descriptor != -1)
//			write(descriptor, line, ft_strlen(line));
//		free(line);
//	}
//}

//void	populate_delim(int descriptor, t_ms_token *token)
//{
//	char		*line;
//	char *const	delimiter = ft_calloc(get_delim(token, NULL) + 1, sizeof(char));
//
//	get_delim(token, delimiter);
//	while (true)
//	{
//		line = readline(NULL);
//		if (line == NULL || ft_strcmp(line, delimiter) == 0)
//		{
//			free(line);
//			free(delimiter);
//			return ;
//		}
//		if (descriptor != -1)
//		{
//			write(descriptor, line, ft_strlen(line));
//			write(descriptor, "\n", 1); // ✅ Zeilenumbruch schreiben
//		}
//		free(line);
//	}
//}

void	populate_delim(int descriptor, t_ms_token *token)
{
	char		*line;
	char		*delimiter = get_delim_string(token);

	while (true)
	{
		line = readline(NULL);
		if (line == NULL || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			free(delimiter);
			return ;
		}
		if (descriptor != -1)
		{
			write(descriptor, line, ft_strlen(line));
			write(descriptor, "\n", 1); // ✅ Zeilenumbruch schreiben
		}
		free(line);
	}
}

//int	ms_read_delims(t_ms_token *token, t_ms_token *last)
//{
//	const int			descriptor = gc_add_fd(open("minishell_delim_file.tmp", O_CREAT | O_TRUNC | O_WRONLY | O_RDONLY, 0777));
//	t_ms_token *const	last_delim = get_last_delim(token);
//
//	//printf("ms_read_delims %d\n", descriptor);
//	if (last_delim == NULL)
//		return (-1);
//	while (token != last)
//	{
//		if (token -> index == MS_TOKEN_DELIM)
//	 	{
//			if (token == last_delim)
//				populate_delim(descriptor, token -> next);
//			else
//				populate_delim(-1, token -> next);
//		}
//		token = token -> next;
//	}
//	return (descriptor);
//}

int	ms_read_delims(t_ms_token *token, t_ms_token *last)
{
	t_ms_token *const	last_delim = get_last_delim(token);
	int					write_fd;
	int					read_fd;

	if (last_delim == NULL)
		return (-1);

	// 1. Datei erzeugen und befüllen
	write_fd = open("minishell_delim_file.tmp", O_CREAT | O_TRUNC | O_WRONLY, 0644);
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

	// 2. Jetzt zum Lesen öffnen und zurückgeben
	read_fd = gc_add_fd(open("minishell_delim_file.tmp", O_RDONLY));
	if (read_fd == -1)
		return (-1);

	return (read_fd);
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