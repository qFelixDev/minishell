/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:15:33 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/25 11:28:57 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ms_print_tokens(t_ms_token *token);

bool	expand_variables(t_ms_token *token)
{
	char	*value;

	while (token != NULL)
	{
		if (token -> index == MS_TOKEN_VARIABLE)
		{
			value = getenv(token -> content);
			if (value == NULL)
				value = "";
			value = ft_strdup(value);
			if (value == NULL)
				return (false);
			free(token -> content);
			token -> index = MS_TOKEN_STRING;
			token -> content = value;
		}
		token = token -> next;
	}
	return (true);
}

bool	ruin_delimiter(t_ms_token *token)
{
	char	*content;

	if (token -> index == MS_TOKEN_WILDCARD)
	{
		token -> content = ft_strdup("*");
		if (content == NULL)
			return (false);
		token -> index = MS_TOKEN_STRING;
	}
	if (token -> index == MS_TOKEN_VARIABLE)
	{
		content = malloc(ft_strlen(token -> content) + 2);
		if (content == NULL)
			return (false);
		content[0] = '$';
		ft_strlcpy(content + 1, token -> content, ft_strlen(token -> content) + 1);
		free(token -> content);
		token -> content = content;
		token -> index = MS_TOKEN_STRING;
	}
	return (true);
}

bool	ruin_delimiters(t_ms_token *token)
{
	char	*string;
	bool	is_ruining;

	while (token != NULL)
	{
		if (token -> index < MS_TOKEN_WILDCARD)
			is_ruining = false;
		if (is_ruining) {
			if (!ruin_delimiter(token))
				return (false);
			//puts("ruin");
		}
		if (!token -> concatenate_content)
			is_ruining = false;
		if (token -> index == MS_TOKEN_DELIM)
			is_ruining = true;
		token = token -> next;
	}
	return (true);
}

void	get_pattern_quantities(
	t_ms_token *first, size_t *pattern_length_pt, size_t *asterisk_count_pt)
{
	char	*string;

	*pattern_length_pt = 0;
	*asterisk_count_pt = 0;
	while (token != NULL)
	{
		if (token -> index == MS_TOKEN_STRING)
		{
			string = token -> content;
			while (*string != '\0')
			{
				(*pattern_length_pt)++;
				*asterisk_count_pt += (*string == '*');
				string++;
			}
		}
		*asterisk_count_pt += (token -> index == MS_TOKEN_WILDCARD
			&& (token -> next == NULL
				|| token -> next -> index == MS_TOKEN_STRING
				|| !token -> concatenate_content));
		if (!token -> concatenate_content)
			break ;
		token = token -> next;
	}
}

size_t	add_match(t_ms_pattern *pattern)
{
	char *const	new_path = ft_strjoin()
	struct stat	status;
}

size_t	ms_enumerate_matches(t_ms_pattern *pattern, char *path, char **output)
{
	DIR *const		directory = opendir(path);
	struct dirent	*entry;
	size_t			match_count;
	size_t			result;

	if (directory == NULL)
		return (SIZE_MAX);
	match_count = 0;
	entry = readdir(directory);
	while (entry != NULL)
	{
		if (matches_pattern(pattern, entry -> d_name))
		{
			result = add_match();
			if (result == SIZE_MAX)
				return (SIZE_MAX);
			match_count += result;
		}
		entry = readdir(directory);
	}
	free(path);
	closedir(directory);
	return (match_count);
}

size_t	ms_enumerate_matches(t_ms_pattern *pattern, char *path, char **output)
{
	DIR *const		directory = opendir(path);
	struct dirent	*entry;
	size_t			match_count;
	size_t			result;

	if (directory == NULL)
		return (SIZE_MAX);
	match_count = 0;
	entry = readdir(directory);
	while (entry != NULL)
	{
		if (matches_pattern(pattern, entry -> d_name))
		{
			result = add_match();
			if (result == SIZE_MAX)
				return (SIZE_MAX);
			match_count += result;
		}
		entry = readdir(directory);
	}
	return (match_count);
}

int	count_arguments(t_ms_token *token)
{
	size_t			result;
	int				argument_count;
	t_ms_pattern	*pattern;

	argument_count = -ms_count_index(token, MS_TOKEN_DELIM);
	argument_count -= ms_count_index(token, MS_TOKEN_APPEND);
	argument_count -= ms_count_index(token, MS_TOKEN_INPUT);
	argument_count -= ms_count_index(token, MS_TOKEN_OUTPUT);
	if (token -> index >= MS_TOKEN_WILDCARD)
		if (!ms_enumerate_matches(token, NULL))
			return (-1);
	while (token -> next != NULL)
	{
		if (!token -> concatenate_content
			&& token -> next -> index >= MS_TOKEN_WILDCARD)
			if (!ms_enumerate_matches(token, NULL))
				return (-1);
		token = token -> next;
	}
	return (argument_count);
}

void	ms_free_command(t_ms_command *command)
{
	int	index;

	if (command == NULL)
		return ;
	if (command -> argv != NULL)
	{
		index = 0;
		while (command -> argv[index] != NULL)
			free(command -> argv[index++]);
	}
	if (command -> redirects != NULL)
	{
		index = -1;
		while (++index < 4)
		{
			if (command -> redirects[index] == NULL)
				continue ;
			file_index = 0;
			while (command -> redirects[index][file_index] != NULL)
				free(command -> redirects[index][file_index++]);
		}
	}
}

t_ms_command	*ms_allocate_command(t_ms_token *token)
{
	t_ms_command	*command;
	const int		argument_count = count_arguments(token);
	int				redirect_index;

	if (argument_count == -1)
		return (NULL);
	command = ft_calloc(1, sizeof(t_ms_command));
	if (command == NULL)
		return (NULL);
	command -> argv = malloc(argument_count);
	if (command -> argv == NULL)
		return (ms_free_command(command), NULL);
	redirect_index = 0;
	while (redirect_index < 4)
	{
		command -> redirects[redirect_index] = ft_calloc(
			ms_count_index(token, MS_TOKEN_DELIM + redirect_index) + 1,
			sizeof(char *));
		if (command -> redirects[redirect_index] == NULL)
			return (ms_free_command(command), NULL);
		redirect_index++;
	}
	return (command);
}

bool	check_ambiguity(t_ms_token *token)
{
	while (token != NULL)
	{
		if (token -> index >= MS_TOKEN_APPEND
			&& token -> index <= MS_TOKEN_OUTPUT
			&& ms_enumerate_matches(token, NULL) != 1)
			return (false);
		token = token -> next;
	}
	return (true);
}

t_ms_token	*next_pattern_token(t_ms_token *token)
{
	if (token -> index >= MS_TOKEN_DELIM && token -> index <= MS_TOKEN_OUTPUT)
		token = token -> next;
	while (token != NULL && token -> index >= MS_TOKEN_WILDCARD
		&& token -> concatenate_content)
		token = token -> next;
	if (token != NULL && token -> index >= MS_TOKEN_WILDCARD)
		token = token -> next;
	return (token);
}

bool	populate_command(t_ms_command *command, t_ms_token *token)
{
	int	argument_index;
	int	redirect_indices[4];

	argument_index = 0;
	ft_bzero(redirect_indices, 4 * sizeof(int));
	while (token != NULL)
	{
		if (token -> index >= MS_TOKEN_DELIM
			&& token -> index <= MS_TOKEN_OUTPUT)
			if (ms_enumerate_matches(token -> next, command -> redirects
				[token->index] + redirect_indices[token->index]++) == SIZE_MAX)
				return (false);
		else
		{
			result = ms_enumerate_matches(token,
				command -> argv[argument_index]);
			if (result == SIZE_MAX)
				return (false);
			argument_index += result;
		}
		token = next_pattern_token(token);
	}
	return (true);
}

t_ms_command	*ms_get_command(t_ms_token *token)
{
	t_ms_command	*command;

	ms_print_tokens(token);
	if (!ruin_delimiters(token))
		return (NULL);
	ms_print_tokens(token);
	if (!expand_variables(token))
		return (NULL);
	ms_print_tokens(token);
	if (!check_ambiguity(token))
		return (NULL);
	command = allocate_command(token);
	if (command == NULL)
		return (NULL);
	if (!populate_command(command, token))
		return (ms_free_command(command), NULL);
	return (command);
}
