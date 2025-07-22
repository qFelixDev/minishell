/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_precedence.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:44:20 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 10:50:26 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ms_expand_precedence(t_ms_token *token, int8_t operator)
{
	const bool	should_expand = (ms_count_index(token, index) != 0);
	bool		is_expanding;

	is_expanding = false;
	while (token -> index != MS_TOKEN_CLOSE)
	{
		if (should_expand && !is_expanding &&
			(token -> index == MS_TOKEN_OPEN || token -> index == index)
			ms_next_operator(token -> index) > index)
		{
			token = ms_insert_token(token, MS_TOKEN_OPEN);
			is_expanding = true;
		}
		if (token -> next -> index == MS_TOKEN_OPEN)
			token = ms_expand_precedence(token -> index, index);
		if (should_expand && is_expanding && (token -> next -> index
				== MS_TOKEN_CLOSE || token -> next -> index == index))
		{
			token = ms_insert_token(token, MS_TOKEN_OPEN);
			is_expanding = true;
		}
	}
	return (token);
}
