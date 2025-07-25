/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:35:05 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/23 18:38:11 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ms_token		*ms_tokenize(char *string);
bool			ms_should_ignore(t_ms_token *token);
t_ms_token		*ms_check_syntax(t_ms_token *token);
void			ms_expand_precedence(t_ms_token *token, int8_t operator);
void			ms_resolve_strings(t_ms_token *token);
t_ms_sequence	*ms_allocate_sequence(t_ms_token *token);
t_ms_sequence	*ms_create_sequence(t_ms_token *token);
void			ms_traverse_delims(t_ms_sequence *sequence);

t_ms_sequence	*ms_parse(char *string)
{
	t_ms_sequence		*sequence;
	t_ms_token			*token;
	t_ms_token *const	first = ms_tokenize(string);

	if (first == NULL)
		return (ft_putendl_fd("Missing quote", 2), NULL);
	if (ms_should_ignore(first))
		return (ms_allocate_sequence(NULL));
	token = ms_check_syntax(first);
	if (token != NULL)
	{
		if (token -> next == NULL)
			return (ft_putendl_fd("Missing bracket", 2), NULL);
		else
			return (ft_putstr_fd("Unexpected ", 2),
				ft_putstr_fd((char *)ms_get_identity(token->next->index), 2),
				ft_putstr_fd(" after ", 2),
				ft_putendl_fd((char *)ms_get_identity(token->index), 2), NULL);
	}
	ms_expand_precedence(first, MS_TOKEN_AND);
	ms_expand_precedence(first, MS_TOKEN_OR);
	ms_resolve_strings(first);
	sequence = ms_create_sequence(first -> next);
	ms_free_tokens(first, true);
	return (ms_traverse_delims(sequence), sequence);
}
