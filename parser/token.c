/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:04:46 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/14 12:28:23 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "token.h"

size_t	ms_count_index(t_ms_token *token, int8_t index)
{
	int		bracket_level;
	size_t	count;

	bracket_level = 0;
	count = 0;
	token = token -> next;
	while (bracket_level >= 0)
	{
		count += (bracket_level == 0 && token -> index == index);
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = token -> next;
	}
	return (count);
}

const char	*ms_get_identity(int8_t index)
{
	static char	*token_identities[MS_TOKEN_MAX] = {"&&", "||", "|", "(",
		")", "<<", ">>", "<", ">", "*", "$STR", "\"STR\"", "\'STR\'", "~STR~"};

	if (index == MS_TOKEN_NONE)
		index = MS_TOKEN_AND;
	return (token_identities[index]);
}

void	ms_print_tokens(t_ms_token *token)
{
	while (token != NULL)
	{
		printf("%s ", ms_get_identity(token -> index));
		token = token -> next;
	}
	printf("\n");
}

void	ms_free_tokens(t_ms_token *token, bool detach_only)
{
	t_ms_token	*temp;

	while (token != NULL)
	{
		temp = token;
		token = token -> next;
		if (detach_only &&
			(token == NULL || token -> index < MS_TOKEN_DELIM))
			temp -> next = NULL;
		if (!detach_only && temp -> index >= MS_TOKEN_VARIABLE)
			free(temp -> content);
		if (!detach_only || temp -> index < MS_TOKEN_DELIM)
			free(temp);
	}
}

int8_t	ms_next_operator(t_ms_token *token)
{
	int	bracket_level;

	bracket_level = 0;
	while (token != NULL && bracket_level >= 0)
	{
		if (bracket_level == 0 && token -> index <= MS_TOKEN_PIPE)
			return (token -> index);
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = token -> next;
	}
	return (MS_TOKEN_NONE);
}
