/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:04:46 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/09 12:06:34 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

enum
{
	MS_TOKEN_NONE = -1,
	MS_TOKEN_AND,
	MS_TOKEN_OR,
	MS_TOKEN_PIPE,
	MS_TOKEN_OPEN,
	MS_TOKEN_CLOSE,
	MS_TOKEN_DELIM,
	MS_TOKEN_APPEND,
	MS_TOKEN_INPUT,
	MS_TOKEN_OUTPUT,
	MS_TOKEN_VARIABLE,
	MS_TOKEN_WILDCARD,
	MS_TOKEN_UNRESOLVED_STRING,
	MS_TOKEN_STRING
}

size_t	ms_count_index(t_ms_token *token, int8_t index)
{
	int		bracket_level;
	size_t	count;

	bracket_level = 0;
	count = 0;
	token = token -> next;
	while (bracket_level >= 0)
	{
		count += (bracket_level == 0 && token -> index == index);
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = token -> next;
	}
	return (count);
}