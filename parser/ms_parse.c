/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:35:05 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/23 15:27:52 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ms_token		*ms_tokenize(char *string);
t_ms_token		*ms_check_syntax(t_ms_token *token);
void			ms_expand_precedence(t_ms_token *token, int8_t operator);
void			ms_resolve_strings(t_ms_token *token);
t_ms_sequence	*ms_create_sequence(t_ms_token *token);
void			ms_traverse_delims(t_ms_sequence *sequence);

t_ms_sequence	*ms_parse(char *string)
{
	t_ms_sequence		*sequence;
	t_ms_token			*token;
	t_ms_token *const	first = ms_tokenize(string);

	//ms_print_tokens(first);
	if (first == NULL)
		return (fprintf(stderr, "Missing quote\n"), NULL);
	token = ms_check_syntax(first);
	if (token != NULL)
	{
		if (token -> next == NULL)
			return (fprintf(stderr, "Missing bracket\n"), NULL);
		else
			return (fprintf(stderr, "Unexpected %s after %s\n",
					ms_get_identity(token -> next -> index),
					ms_get_identity(token -> index)), NULL);
	}
	//ms_print_tokens(first);
	ms_expand_precedence(first, MS_TOKEN_AND);
	//ms_print_tokens(first);
	ms_expand_precedence(first, MS_TOKEN_OR);
	//ms_print_tokens(first);
	ms_resolve_strings(first);
	sequence = ms_create_sequence(first -> next);
	ms_free_tokens(first, true);
	ms_traverse_delims(sequence);
	return (sequence);
}
