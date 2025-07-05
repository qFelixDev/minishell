/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_precedence.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:08:12 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/05 10:51:36 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_extend/libft.h"
#include "token.h"

static t_ms_token	*add_bracket(
	t_ms_token *token, int8_t index, bool *should_expand_pt)
{
	*should_expand_pt = !*should_expand_pt;
	return (ms_insert_token(token, index));
}

t_ms_token	*ms_expand_precedence(t_ms_token *token, int8_t index)
{
	const bool	should_expand = (ms_count_index(token, index) != 0);
	bool		is_expanding;

	is_expanding = false;
	while (token -> index != MS_TOKEN_CLOSE)
	{
		if (should_expand && !is_expanding && ms_next_operator(token->next) > index
			&& (token->index == MS_TOKEN_OPEN || token->index == index))
			token = add_bracket(token, MS_TOKEN_OPEN, &should_expand);
		if (token == NULL)
			return (NULL);
		if (token -> next -> index == MS_TOKEN_OPEN)
		{
			token = ms_expand_precedence(token -> next, index);
			if (token == NULL)
				return (NULL);
		}
		if (should_expand && is_expanding
			&& (token->next->index == MS_TOKEN_CLOSE || token->next->index == index))
			token = add_bracket(token, MS_TOKEN_CLOSE, &should_expand);
		if (token == NULL)
			return (NULL);
		token = token -> next;
	}
	return (token);
}
