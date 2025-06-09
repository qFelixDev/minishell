/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:13:11 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/08 14:08:46 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_ms_token	*ms_tokenize(char *string)
{
	t_ms_token *const	first = malloc(sizeof(t_ms_token));
	t_ms_token			*token;

	if (first == NULL)
		return (NULL);
	first -> index = MS_TOKEN_BRACKET_OPEN;
	token = first;
	while (true)
	{
		token -> next = malloc(sizeof(t_ms_token));
		token = token -> next;
		if (token == NULL)
			return (ms_free_tokens(first), NULL);
		string = populate_token(token, string);
		if (string == NULL)
			return (ms_free_tokens(first), NULL);
	}
	if (ms_insert_token(token, MS_TOKEN_BRACKET_CLOSE) == NULL)
		return (ms_free_tokens(first), NULL);
	return (first);
}

