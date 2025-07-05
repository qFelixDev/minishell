/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:15:33 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/05 13:01:21 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define MS_TOKENS
#include "../includes/minishell.h"
#include "token.h"

void	ms_print_tokens(t_ms_token *token);
size_t	ms_expand_wildcards(t_ms_token *token, char **paths);

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
		if (token -> content == NULL)
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
	bool	is_ruining;

	while (token != NULL)
	{
		if (token -> index < MS_TOKEN_WILDCARD)
			is_ruining = false;
		if (is_ruining)
			if (!ruin_delimiter(token))
				return (false);
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
	size_t	result;
	int		argument_count;

	argument_count = -ms_count_index(token, MS_TOKEN_DELIM);
	argument_count -= ms_count_index(token, MS_TOKEN_APPEND);
	argument_count -= ms_count_index(token, MS_TOKEN_INPUT);
	argument_count -= ms_count_index(token, MS_TOKEN_OUTPUT);
	if (token -> index >= MS_TOKEN_WILDCARD)
	{
		result = ms_expand_wildcards(token, NULL);
		if (result == SIZE_MAX)
			return (-1);
		argument_count += result;
	}
	while (token -> next != NULL)
	{
		if (!token -> concatenate_content
			&& token -> next -> index >= MS_TOKEN_WILDCARD)
		{
			result = ms_expand_wildcards(token -> next, NULL);
			if (result == SIZE_MAX)
				return (-1);
			argument_count += result;
		}
		token = token -> next;
	}
	return (argument_count);
}

void	ms_free_command(t_ms_command *command)
{
	int	index;
	int	file_index;

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
	command -> argv = ft_calloc(sizeof(char *), argument_count + 1);
	if (command -> argv == NULL)
		return (ms_free_command(command), NULL);
	command -> redirects = ft_calloc(sizeof(char **), 4);
	if (command -> redirects == NULL)
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
	size_t	result;

	if (token == NULL)
		return (true);
	while (token -> next != NULL)
	{
		if (token -> index >= MS_TOKEN_APPEND
			&& token -> index <= MS_TOKEN_OUTPUT)
		{
			result = ms_expand_wildcards(token -> next, NULL);
			if (result != 1)
			{
				printf("Ambiguous redirect: %zu\n", result);
				return (false);
			}
		}
		token = token -> next;
	}
	return (true);
}

bool	populate_command(t_ms_command *command, t_ms_token *token)
{
	int		argument_index;
	int		redirect;
	size_t	result;
	int		redirect_indices[4];

	argument_index = 0;
	ft_bzero(redirect_indices, sizeof(redirect_indices));
	while (token != NULL)
	{
		if (token -> index >= MS_TOKEN_WILDCARD)
		{
			result = ms_expand_wildcards(token, command -> argv + argument_index);
			if (result == SIZE_MAX)
				return (false);
			argument_index += result;
		}
		else
		{
			redirect = token -> index - MS_TOKEN_DELIM;
			token = token -> next;
			if (ms_expand_wildcards(token, command -> redirects[redirect] + redirect_indices[redirect]++) == SIZE_MAX)
				return (false);
		}
		while (token != NULL && token -> concatenate_content)
			token = token -> next;
		if (token != NULL)
			token = token -> next;
	}
	return (true);
}

t_ms_command	*ms_get_command(t_ms_token *token)
{
	t_ms_command	*command;

	ms_print_tokens(token);
	if (!ruin_delimiters(token)) {
		puts("Unable to ruin delimiters");
		return (NULL);
	}
	ms_print_tokens(token);
	if (!expand_variables(token)) {
		puts("Variable expansion failed");
		return (NULL);
	}
	ms_print_tokens(token);
	if (!check_ambiguity(token))
		return (NULL);
	command = ms_allocate_command(token);
	if (command == NULL) {
		puts("Command allocation failed");
		return (NULL);
	}
	if (!populate_command(command, token))
		return (ms_free_command(command), NULL);
	int argument_count = 0;
	while(command -> argv[argument_count] != NULL)
		argument_count++;
	printf("%d\n", argument_count);
	return (command);
}
