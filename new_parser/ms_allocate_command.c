/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_allocate_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 12:48:33 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 19:36:34 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ms_expand_wildcards(t_ms_token *token, char **paths);

int	count_arguments(t_ms_token *token)
{
	int			argument_count;
	uint32_t	concatenation;

	argument_count = -ms_count_index(token, MS_TOKEN_DELIM);
	argument_count -= ms_count_index(token, MS_TOKEN_APPEND);
	argument_count -= ms_count_index(token, MS_TOKEN_INPUT);
	argument_count -= ms_count_index(token, MS_TOKEN_OUTPUT);
	while (token != NULL)
	{
		concatenation = token -> concatenation;
		argument_count += ms_expand_wildcards(token, NULL);
		while (token != NULL && token -> concatenation == concatenation)
			token = token -> next;
	}
	return (argument_count);
}

t_ms_command	*ms_allocate_command(t_ms_token *token)
{
	t_ms_command *const	command = gc_add(ft_calloc(1, sizeof(t_ms_command)));
	const int			argument_count = count_arguments(token);
	int					redirect_index;

	command -> argv
		= gc_add(ft_calloc(argument_count + 1, sizeof(char *)));
	command -> redirects
		= gc_add(ft_calloc(MS_REDIRECT_MAX, sizeof(char **)));
	redirect_index = 0;
	while (redirect_index < MS_REDIRECT_MAX)
	{
		command -> redirects[redirect_index] = gc_add(ft_calloc(
				ms_count_index(token, MS_TOKEN_DELIM + redirect_index) + 1,
				sizeof(char *)));
		redirect_index++;
	}
	return (command);
}
