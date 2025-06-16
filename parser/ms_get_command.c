/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:15:33 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/16 13:34:56 by ghodges          ###   ########.fr       */
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

t_ms_command	*ms_get_command(t_ms_token *token)
{
	t_ms_command	*command;

	puts("getting command");
	ms_print_tokens(token);
	if (!ruin_delimiters(token))
		return (NULL);
	ms_print_tokens(token);
	if (!expand_variables(token))
		return (NULL);
	ms_print_tokens(token);
	return (NULL);
}
