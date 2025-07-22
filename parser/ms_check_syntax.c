/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_check_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:55:30 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 15:23:28 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define BRANCHES_AND				0b00010111111111
#define BRANCHES_OR					0b00010111111111
#define BRANCHES_PIPE				0b00010111111111
#define BRANCHES_OPEN				0b00010111111111
#define BRANCHES_CLOSE				0b11101000000000
#define BRANCHES_DELIM				0b00000000011111
#define BRANCHES_APPEND				0b00000000011111
#define BRANCHES_INPUT				0b00000000011111
#define BRANCHES_OUTPUT				0b00000000011111
#define BRANCHES_WILDCARD			0b11101111111111
#define BRANCHES_VARIABLE			0b11101111111111
#define BRANCHES_UNRESOLVED_STRING	0b11101111111111
#define BRANCHES_STRING				0b11101111111111
#define BRANCHES_SHADOW_STRING		0b11101111111111

t_ms_token	*ms_check_syntax(t_ms_token *token)
{
	const uint16_t	token_branches[MS_TOKEN_MAX] = {
		BRANCHES_AND, BRANCHES_OR, BRANCHES_PIPE, BRANCHES_OPEN,
		BRANCHES_CLOSE, BRANCHES_DELIM, BRANCHES_APPEND, BRANCHES_INPUT,
		BRANCHES_OUTPUT, BRANCHES_WILDCARD, BRANCHES_VARIABLE,
		BRANCHES_UNRESOLVED_STRING, BRANCHES_STRING, BRANCHES_SHADOW_STRING
	};
	t_ms_token		*next_token;
	int				bracket_level;

	bracket_level = 0;
	while (token != NULL)
	{
		next_token = token -> next;
		if (next_token != NULL && !(token_branches[token -> index]
				& (0x2000 >> next_token -> index)))
			return (token);
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = next_token;
	}
	//if (bracket_level != 0)
	//	return (token);
	return (NULL);
}
