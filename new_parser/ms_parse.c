/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:35:05 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 10:39:38 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_ms_sequence	*ms_parse(char *string)
{
	t_ms_token *const	first = ms_tokenize(string);

	if (token == NULL)
		return (fprintf(stderr, "Missing quote\n"), NULL);
	token = ms_check_syntax(first);
	if (token != NULL)
	{
		if (token -> next == NULL)
			return (fprintf(stderr, "Missing bracket\n"), NULL);
		else
			return (fprintf(stderr, "Unexpected %s after %s\n",
					ms_get_identity(token -> next -> index),
					ms_get_identity(token -> index)));
	}
	ms_expand_precedence(first, MS_TOKEN_AND);
	ms_expand_precedence(first, MS_TOKEN_OR);
	ms_resolve_strings(first);
}
