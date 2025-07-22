/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sequence.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:48:34 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 15:18:49 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_ms_token	*next_object(t_ms_token *token)
{
	int	bracket_level;

	if (token -> index >= MS_TOKEN_DELIM)
	{
		while (token -> index >= MS_TOKEN_DELIM)
			token = token -> next;
		return (token -> next);
	}
	bracket_level = 1;
	while (bracket_level > 0)
	{
		token = token -> next;
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
	}
	if (token -> next != NULL)
		token = token -> next;
	return (token -> next);
}

t_ms_sequence	*ms_allocate_sequence(t_ms_token *token)
{
	int						bracket_level;
	t_ms_sequence *const	sequence
		= gc_add(ft_calloc(1, sizeof(t_ms_sequence)));

	if (token == NULL)
		return (sequence);
	sequence -> object_count = 1;
	bracket_level = 0;
	while (bracket_level >= 0)
	{
		if (bracket_level == 0 && token -> index <= MS_TOKEN_PIPE)
			sequence -> object_count++;
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = token -> next;
	}
	sequence -> objects = gc_add(
		ft_calloc(sizeof(void *), sequence -> object_count + 1));
	sequence -> is_sequence = gc_add(
		ft_calloc(sizeof(uint8_t) * (sequence -> object_count / 8 + 1), 1));
	sequence -> delim_descriptors = gc_add(
		ft_calloc(sizeof(int), sequence -> object_count + 1));
	return (sequence);
}

t_ms_sequence	*ms_create_sequence(t_ms_token *token)
{
	size_t					object_index;
	t_ms_sequence *const	sequence = ms_allocate_sequence(token);

	if (sequence == NULL)
		return (NULL);
	sequence -> operator = ms_next_operator(token);
	object_index = 0;
	while (object_index < sequence -> object_count && token != NULL)
	{
		if (token -> index == MS_TOKEN_OPEN)
		{
			sequence -> objects[object_index]
				= ms_create_sequence(token -> next);
			sequence -> is_sequence[object_index / 8]
				|= (1u << (object_index % 8));
		}
		else
			sequence -> objects[object_index] = token;
		token = next_object(token);
		object_index++;
	}
	return (sequence);
}

void	ms_free_sequence(t_ms_sequence *sequence)
{
	size_t	object_index;

	object_index = 0;
	while (object_index < sequence -> object_count)
	{
		if (sequence -> objects[object_index] != NULL)
		{
			if (sequence -> is_sequence[object_index / 8]
				& (1u << (object_index % 8)))
				ms_free_sequence(sequence -> objects[object_index]);
			else
				ms_free_tokens(sequence -> objects[object_index], false);
		}
		object_index++;
	}
	gc_free_ptr(sequence -> objects);
	gc_free_ptr(sequence -> is_sequence);
	gc_free_ptr(sequence);
}