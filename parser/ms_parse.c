/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:35:05 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/23 16:29:44 by reriebsc         ###   ########.fr       */
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

	if (first == NULL)
		return (ft_putendl_fd("Missing quote", stderr), NULL);
	token = ms_check_syntax(first);
	if (token != NULL)
	{
		if (token -> next == NULL)
			return (ft_putendl_fd("Missing bracket", stderr), NULL);
		else
			return (fprintf(stderr, "Unexpected %s after %s\n",
					ms_get_identity(token -> next -> index),
					ms_get_identity(token -> index)), NULL);
	}
	ms_expand_precedence(first, MS_TOKEN_AND);
	ms_expand_precedence(first, MS_TOKEN_OR);
	ms_resolve_strings(first);
	sequence = ms_create_sequence(first -> next);
	ms_free_tokens(first, true);
	ms_traverse_delims(sequence);
	return (sequence);
}
