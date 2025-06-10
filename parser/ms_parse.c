/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:54:02 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/09 13:48:15 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_ms_sequence	*ms_parse(char *string)
{
	const char *const	token_identities[MS_TOKEN_SHADOW_STRING] = {"&&", "||",
		"|", "(", ")", "<<", ">>", "<", ">", "*", "$STR", "\"STR\"", "STR"};
	t_ms_token *const	first = ms_tokenize(string);
	t_ms_token			*token;
	t_ms_sequence		*sequence;

	if (first == NULL)
		return (printf("Tokenization failed\n"), NULL);
	token = ms_check_syntax(first);
	if (token == NULL)
		return (printf("Unexpected %s after %s\n",
			token_identities[token -> index],
			token_identities[token -> next -> index]),
			ms_free_tokens(first), NULL);
	if (ms_expand_precedence(first, MS_TOKEN_AND) == NULL)
		return (printf("Expansion failed\n"), ms_free_tokens(first), NULL);
	if (ms_expand_precedence(first, MS_TOKEN_OR) == NULL)
		return (printf("Expansion failed\n"), ms_free_tokens(first), NULL);
	if (!ms_resolve_strings(first))
		return (printf("Resolution failed\n"), ms_free_tokens(first), NULL);
	sequence = ms_create_sequence(first);
	ms_detach_tokens(first);
	return (sequence);
}
