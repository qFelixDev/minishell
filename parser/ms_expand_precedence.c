/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_precedence.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:08:12 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/14 15:43:25 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_extend/libft.h"
#include "token.h"

static t_ms_token	*add_bracket(
	t_ms_token *token, int8_t index, bool *is_expanding_pt)
{
	if ((token -> index == MS_TOKEN_OPEN || token -> index == index)
		&& !*is_expanding_pt && ms_next_operator(token -> next) > index)
	{
		token = ms_insert_token(token, MS_TOKEN_OPEN);
		if (token == NULL)
			return (NULL);
		*is_expanding_pt = true;
	}
	if ((token -> next -> index == MS_TOKEN_CLOSE
		|| token -> next -> index == index) && *is_expanding_pt)
	{
		token = ms_insert_token(token, MS_TOKEN_CLOSE);
		if (token == NULL)
			return (NULL);
		*is_expanding_pt = false;
	}
	return (token);
}

t_ms_token	*ms_expand_precedence(t_ms_token *token, int8_t index)
{
	const bool	should_expand = (ms_count_index(token, index) != 0);
	bool		is_expanding;

	is_expanding = false;
	while (token -> index != MS_TOKEN_CLOSE)
	{
		if (should_expand)
			token = add_bracket(token, index, &is_expanding);
		if (token == NULL)
			return (NULL);
		token = token -> next;
		if (token -> index != MS_TOKEN_OPEN)
			continue ;
		token = ms_expand_precedence(token, index);
		if (token == NULL)
			return (NULL);
		token = token -> next;
	}
	return (token);
}

