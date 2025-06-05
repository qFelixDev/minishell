/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:00:12 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/04 18:13:31 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "token.h"

#define BRANCHES_AND				0b000111110111
#define BRANCHES_OR					0b000111110111
#define BRANCHES_PIPE				0b000111110111
#define BRANCHES_DELIM				0b000000000111
#define BRANCHES_APPEND				0b000000000111
#define BRANCHES_INPUT				0b000000000111
#define BRANCHES_OUTPUT				0b000000000111
#define BRANCHES_OPEN				0b000111110111
#define BRANCHES_CLOSE				0b111000001000
#define BRANCHES_STRING_OPAQUE		0b111111101111
#define BRANCHES_STRING_SEMI		0b111111101111
#define BRANCHES_STRING_TRANSPARENT	0b111111101111

t_ms_token	*check_syntax(t_ms_token *first)
{
	uint16_t const	token_branches[MS_TOKEN_MAX] = {
		BRANCHES_AND, BRANCHES_OR, BRANCHES_PIPE, BRANCHES_DELIM,
		BRANCHES_APPEND, BRANCHES_INPUT, BRANCHES_OUTPUT, BRANCHES_OPEN,
		BRANCHES_CLOSE, BRANCHES_STRING_OPAQUE, BRANCHES_STRING_SEMI,
		BRANCHES_STRING_TRANSPARENT
	};
	t_ms_token		*token;
	t_ms_token		*next_token;
	int				bracket_level;

	token = first;
	bracket_level = 0;
	while (token != NULL)
	{
		next_token = token -> next;
		if (next_token != NULL && !(token_branches[token -> index]
				& (0x800 >> next_token -> index)))
			return (token);
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = next_token;
	}
	if (bracket_level != 0)
		return (first);
	return (NULL);
}

t_ms_token	*ms_tokenize(char *string);
t_ms_token	*ms_expand_precedence(t_ms_token *token, int8_t index);
t_ms_sequence	*ms_create_sequence(t_ms_token **token_pt);

t_ms_sequence	*ms_parse(char *string)
{
	t_ms_token		*first;
	t_ms_token		*token;
	t_ms_sequence	*sequence;

	first = ms_tokenize(string);
	if (first == NULL)
		return (NULL);
	token = check_syntax(first);
	if (token != NULL)
		return (ms_free_tokens(first), NULL);
	if (ms_contains_index(first, MS_TOKEN_AND)
		&& !ms_expand_precedence(first, MS_TOKEN_AND))
		return (ms_free_tokens(first), NULL);
	if (ms_contains_index(first, MS_TOKEN_OR)
		&& !ms_expand_precedence(first, MS_TOKEN_OR))
		return (ms_free_tokens(first), NULL);
	token = first;
	sequence = ms_create_sequence(&token);
	ms_free_tokens(first);
	return (sequence);
}
