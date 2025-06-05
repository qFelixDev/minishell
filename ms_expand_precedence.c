/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_precedence.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:31:48 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/05 08:32:37 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include<stdio.h>
#include "token.h"

static t_ms_token	*add_bracket(
	t_ms_token *token, int8_t index, bool *in_bracket_pt)
{
	if ((token -> index == MS_TOKEN_CLOSE || token -> index == index)
		&& !*in_bracket_pt && ms_next_operator(token -> next) > index)
	{
		if (!ms_insert_token(token, MS_TOKEN_OPEN))
			return (NULL)
		*in_bracket_pt = true;
		token = token -> next;
	}
	if ((token -> next -> index == MS_TOKEN_CLOSE
		|| token -> next -> index == index) && *in_bracket_pt)
	{
		if (!ms_insert_token(token, MS_TOKEN_CLOSE))
			return (NULL);
		*in_bracket_pt = false;
		token = token -> next;
	}
	return (token);
}

t_ms_token	*ms_expand_precedence(t_ms_token *token, int8_t index)
{
	bool	in_bracket;

	in_bracket = false;
	while (token -> index != MS_TOKEN_CLOSE )
	{
		token = add_bracket(token, index, &in_bracket);
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
