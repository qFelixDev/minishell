/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:47:32 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/04 18:04:25 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "token.h"

int8_t	ms_next_operator(t_ms_token *token)
{
	int	bracket_level;

	bracket_level = 0;
	while (token != NULL && bracket_level >= 0)
	{
		if (bracket_level == 0 && (token -> index == MS_TOKEN_AND
				|| token -> index == MS_TOKEN_OR
				|| token -> index == MS_TOKEN_PIPE))
			return (token -> index);
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = token -> next;
	}
	return (-1);
}

void	ms_free_tokens(t_ms_token *token)
{
	t_ms_token	*temp;

	while (token != NULL)
	{
		temp = token;
		token = token -> next;
		if (temp -> index >= MS_TOKEN_STRING_OPAQUE)
			free(temp -> content);
		free(temp);
	}
}

bool	ms_insert_token(t_ms_token *predecessor, int8_t index)
{
	t_ms_token *const	token = malloc(sizeof(t_ms_token));

	if (token == NULL)
		return (false);
	token -> next = predecessor -> next;
	token -> index = index;
	predecessor -> next = token;
	return (true);
}

bool	ms_contains_index(t_ms_token *token, int8_t index)
{
	while (token != NULL)
	{
		if (token -> index == index)
			return (true);
		token = token -> next;
	}
	return (false);
}