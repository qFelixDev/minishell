/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_precedence.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:31:48 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/04 15:17:15 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include<stdio.h>
#include "token.h"

t_ms_token	*ms_expand_precedence(t_ms_token *token, int8_t index)
{
	bool	in_bracket;

	in_bracket = false;
	while (token -> index != MS_TOKEN_CLOSE)
	{
		if ((token -> index == MS_TOKEN_OPEN || token -> index == index)
			&& !in_bracket && ms_next_operator(token -> next) > index)
		{
			if (!ms_insert_token(token, MS_TOKEN_OPEN))
				return (NULL);
			in_bracket = true;
			token = token -> next;
		}
		if ((token -> next -> index == MS_TOKEN_CLOSE
			|| token -> next -> index == index) && in_bracket)
		{
			if (!ms_insert_token(token, MS_TOKEN_CLOSE))
				return (NULL);
			in_bracket = false;
			token = token -> next;
		}
		token = token -> next;
		if (token -> index == MS_TOKEN_OPEN)
		{
			token = ms_expand_precedence(token, index);
			if (token == NULL)
				return (NULL);
			token = token -> next;
		}
	}
	return (token);
}
