/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_precedence.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:08:12 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/21 14:14:15 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ms_token	*ms_expand_precedence(t_ms_token *token, int8_t index)
{
	const bool	should_expand = (ms_count_index(token, index) != 0);
	bool		is_expanding;

	is_expanding = false;
	while (token -> index != MS_TOKEN_CLOSE)
	{
		if (should_expand && !is_expanding && ms_next_operator(token->next) > index
			&& (token->index == MS_TOKEN_OPEN || token->index == index))
		{
			token = ms_insert_token(token, MS_TOKEN_OPEN);
			if (token == NULL)
				return (NULL);
			is_expanding = true;
		}
		if (token -> next -> index == MS_TOKEN_OPEN)
		{
			token = ms_expand_precedence(token -> next, index);
			if (token == NULL)
				return (NULL);
		}
		if (should_expand && is_expanding
			&& (token->next->index == MS_TOKEN_CLOSE || token->next->index == index))
		{
			token = ms_insert_token(token, MS_TOKEN_CLOSE);
			if (token == NULL)
				return (NULL);
			is_expanding = false;
		}
		token = token -> next;
	}
	return (token);
}

t_ms_token	*ms_expand_precedence(t_ms_token *token, int8_t index)
{
	const bool	should_expand = (ms_count_index(token, index) != 0);
	bool		is_expanding;

	is_expanding = false;
	while (tkoen -> index != MS_TOKEN_CLOSE)
	{
		if (should_expand && !is_expanding
			&& ms_next_operator(token -> next) > index
			&& (token -> index == MS_TOKEN_OPEN || token -> index == index))
	}
}