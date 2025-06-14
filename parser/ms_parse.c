/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:54:02 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/14 10:31:47 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_extend/libft.h"
#include "token.h"

t_ms_token		*ms_tokenize(char *string);
t_ms_token		*ms_check_syntax(t_ms_token *token);
t_ms_token		*ms_expand_precedence(t_ms_token *token, int8_t index);
bool			ms_resolve_strings(t_ms_token *token);
void			ms_free_sequence(t_ms_sequence *sequence);
t_ms_sequence	*ms_create_sequence(t_ms_token *token);
void			ms_print_sequence(t_ms_sequence *sequence, int indentation);

t_ms_sequence	*ms_parse(char *string)
{
	t_ms_token			*token;
	t_ms_sequence		*sequence;
	t_ms_token *const	first = ms_tokenize(string);

	if (first == NULL)
		return (printf("Tokenization failed\n"), NULL);
	token = ms_check_syntax(first);
	if (token != NULL)
		return (printf("Unexpected %s after %s\n",
			ms_get_identity(token -> next -> index),
			ms_get_identity(token -> index)),
			ms_free_tokens(first, false), NULL);
	ms_print_tokens(first);
	if (ms_expand_precedence(first, MS_TOKEN_AND) == NULL)
		return (printf("Expansion failed\n"),
			ms_free_tokens(first, false), NULL);
	ms_print_tokens(first);
	if (ms_expand_precedence(first, MS_TOKEN_OR) == NULL)
		return (printf("Expansion failed\n"),
			ms_free_tokens(first, false), NULL);
	ms_print_tokens(first);
	if (!ms_resolve_strings(first))
		return (printf("Resolution failed\n"),
			ms_free_tokens(first, false), NULL);
	ms_print_tokens(first);
	sequence = ms_create_sequence(first -> next);
	ms_free_tokens(first, true);
	return (sequence);
}

int main() {
	char buffer[1000];
	ft_memset(buffer, 0, 1000);
	while(read(0, buffer, 999) > 0) {
		t_ms_sequence* sequence = ms_parse(buffer);
		if (sequence == NULL) {
			ft_memset(buffer, 0, 1000);
			continue;
		}
		ms_print_sequence(sequence, 0);
		ms_free_sequence(sequence);
		ft_memset(buffer, 0, 1000);
	}
}