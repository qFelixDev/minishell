/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:01:51 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 11:22:23 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_ms_token	*ms_insert_token(t_ms_token *token, int8_t index)
{
	t_ms_token *const	new_token = gc_add(ft_calloc(1, sizeof(t_ms_token)));

	new_token -> index = index;
	if (token != NULL && index >= MS_TOKEN_WILDCARD)
		new_token -> concatenation = MS_FIRST_CONCATENATION;
	if (token == NULL)
		return (new_token);
	new_token -> next = token -> next;
	token -> next = new_token;
	return (new_token);
}

size_t	ms_count_index_at_bracket_level(t_ms_token *token, int8_t index)
{
	int		bracket_level;
	size_t	count;

	if (token == NULL)
		return (0);
	bracket_level = 0;
	count = 0;
	while (token != NULL && bracket_level >= 0)
	{
		count += (bracket_level == 0 && token -> index == index);
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level += (token -> index == MS_TOKEN_CLOSE);
		token = token -> next;
	}
	return (count);
}

int8_t	ms_next_operator(t_ms_token *token)
{
	int	bracket_level;

	bracket_level = 0;
	while (token != NULL && bracket_level >= 0)
	{
		if (bracket_level == 0 && token -> index <= MS_TOKEN_PIPE)
			return (token -> index);
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = token -> next;
	}
	return (MS_TOKEN_NONE);
}

void	ms_free_tokens(t_ms_token *token, bool detach_only)
{
	t_ms_token	*temp;

	while (token != NULL)
	{
		temp = token;
		token = token -> next;
		if (detach_only && token != NULL && token -> index < MS_TOKEN_DELIM)
			temp -> next = NULL;
		if (!detach_only && temp -> index >= MS_TOKEN_VARIABLE)
			gc_free_ptr(temp -> content);
		if (!detach_only || temp -> index < MS_TOKEN_DELIM)
			free(temp);
	}
}