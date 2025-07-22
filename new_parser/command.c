/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:54:37 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 14:58:07 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ms_expand_wildcards(t_ms_token *token, char **paths);
t_ms_command	*ms_allocate_command(t_ms_token *token);

void	ruin_delimiter(t_ms_token *token)
{
	char			*content;
	const uint32_t	concatenation = token -> concatenation;

	while (token -> concatenation == concatenation)
	{
		if (token -> index == MS_TOKEN_WILDCARD)
			token -> content = gc_add(ft_strdup("*"));
		if (token -> index == MS_TOKEN_VARIABLE)
		{
			content = gc_malloc(ft_strlen(token -> content) + 2);
			content[0] = '$';
			ft_strlcpy(content + 1, token -> content,
				ft_strlen(token -> content) + 1);
			gc_free_ptr(token -> content);
			token -> content = content;
		}
		token -> index = MS_TOKEN_STRING;
		token = token -> next;
	}
}

void	expand_variable(t_ms_token *token)
{
	char	*value;

	if (token -> content[0] == '\0' && (token -> next == NULL
			|| token -> concatenation != token -> next -> concatenation))
		value = gc_add(ft_strdup("$"));
	else if (token -> content[0] == '$')
		value = gc_add(ft_itoa(getpid()));
	else if (token -> content[0] == '?')
		value = gc_add(ft_itoa(ms_minishell_get() -> exit_status));
	else
	{
		value = ft_getenv(token -> content);
		if (value == NULL)
			value = "";
		value = gc_add(ft_strdup(value));
	}
}

void	populate_command(t_ms_command *command, t_ms_token *token)
{
	int			argument_index;
	int			redirect;
	int			redirect_indices[4];
	uint32_t	concatenation;

	argument_index = 0;
	ft_bzero(redirect_indices, sizeof(redirect_indices));
	while (token != NULL)
	{
		if (token -> index >= MS_TOKEN_WILDCARD)
			argument_index += ms_expand_wildcards(token,
				command -> argv + argument_index);
		else
		{
			redirect = token -> index - MS_TOKEN_DELIM;
			token = token -> next;
			argument_index += ms_expand_wildcards(token,
				command -> redirects[redirect]
				+ redirect_indices[redirect]++);
		}
		concatenation = token -> concatenation;
		while (token != NULL && token -> concatenation == concatenation)
			token = token -> next;
	}
}

t_ms_command	*ms_get_command(t_ms_token *first)
{
	t_ms_command	*command;
	t_ms_token		*token;

	token = first;
	while (token != NULL)
	{
		if (token -> index >= MS_TOKEN_APPEND
			&& token -> index <= MS_TOKEN_OUTPUT
			&& ms_expand_wildcards(token -> next, NULL) != 1)
			return (fprintf(stderr, "Ambiguous redirect\n"), NULL);
		if (token -> index == MS_TOKEN_DELIM)
			ruin_delimiter(token -> next);
		if (token -> index == MS_TOKEN_VARIABLE)
			expand_variable(token);
		token = token -> next;
	}
	command = ms_allocate_command(token);
	populate_command(command, first);
	return (command);
}

void	ms_free_command(t_ms_command *command)
{
	int	index;
	int	file_index;

	if (command == NULL)
		return ;
	index = 0;
	while (command -> argv[index])
		gc_free_ptr(command -> argv[index++]);
	gc_free_ptr(command -> argv);
	index = 0;
	while (index < MS_REDIRECT_MAX)
	{
		file_index = 0;
		while (command -> redirects[index][file_index] != NULL)
			gc_free_ptr(command -> redirects[index][file_index++]);
		gc_free_ptr(command -> redirects[index]);
		index++;
	}
	gc_free_ptr(command);
}
