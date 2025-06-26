/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:15:33 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/27 00:34:26 by ghodges          ###   ########.fr       */
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
			if (ms_enumerate_matches(token, NULL) == SIZE_MAX)
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

size_t	ms_concatenate_pattern(t_ms_token *token, char *pattern)
{
	size_t	length;

	length = 0;
	while (token != NULL && token -> index >= MS_TOKEN_WILDCARD)
	{
		if (token -> index == MS_TOKEN_STRING && pattern != NULL)
			ft_strlcpy(pattern + length, token -> content,
				ft_strlen(token -> content) + 1);
		if (token -> index == MS_TOKEN_WILDCARD && pattern != NULL)
			pattern[length] = '\1';
		if (token -> index == MS_TOKEN_STRING)
			length += ft_strlen(token -> content);
		length += (token -> index == MS_TOKEN_WILDCARD);
		if (!token -> concatenate_content)
			break ;
		token = token -> next;
	}
	return (length);
}

bool	check_ambiguity(t_ms_token *token)
{
	char	*pattern;

	if (token == NULL)
		return (true);
	while (token -> next != NULL)
	{
		if (token -> index >= MS_TOKEN_APPEND
			&& token -> index <= MS_TOKEN_OUTPUT)
		{
			pattern = malloc(ms_concatenate_pattern(token -> next, NULL));
			if (pattern == NULL)
				return (false);
			ms_concatenate_pattern(token -> next, pattern);
			if (ms_enumerate_matches(pattern, ft_strdup("./"), NULL) != 1)
				return (false);
		}
		token = token -> next;
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
