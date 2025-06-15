/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_create_sequence.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 09:48:24 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/15 12:56:42 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_extend/libft.h"
#include "token.h"

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
	free(sequence -> objects);
	free(sequence -> is_sequence);
	free(sequence);
}

static t_ms_sequence	*allocate_sequence(t_ms_token *token)
{
	int						bracket_level;
	t_ms_sequence *const	sequence = ft_calloc(sizeof(t_ms_sequence), 1);

	if (sequence == NULL)
		return (NULL);
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
	sequence -> objects
		= ft_calloc(sizeof(void*) * sequence -> object_count, 1);
	sequence -> is_sequence
		= ft_calloc(sizeof(uint8_t) * (sequence -> object_count / 8 + 1), 1);
	if (sequence -> objects == NULL || sequence -> is_sequence == NULL)
		return (ms_free_sequence(sequence), NULL);
	return (sequence);
}

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

t_ms_sequence	*ms_create_sequence(t_ms_token *token)
{
	size_t					object_index;
	t_ms_sequence *const	sequence = allocate_sequence(token);

	if (sequence == NULL)
		return (NULL);
	sequence -> operator = ms_next_operator(token);
	object_index = 0;
	while (object_index < sequence -> object_count && token != NULL)
	{
		if (token -> index == MS_TOKEN_OPEN)
		{
			sequence->objects[object_index] = ms_create_sequence(token->next);
			if (sequence -> objects[object_index] == NULL)
				return (ms_free_sequence(sequence), NULL);
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

void	ms_print_sequence(t_ms_sequence *sequence, int indentation)
{
	int		indentation_index;
	size_t	object_index;

	indentation_index = -1;
	while (++indentation_index < indentation)
		printf("    ");
	printf("[%s]\n", ms_get_identity(sequence -> operator));
	object_index = 0;
	while (object_index < sequence -> object_count)
	{
		if (sequence -> is_sequence[object_index / 8]
			& (1u << (object_index % 8)))
			ms_print_sequence(
				sequence -> objects[object_index], indentation + 1);
		else
		{
			indentation_index = -2;
			while (++indentation_index < indentation)
				printf("    ");
			ms_print_tokens(sequence -> objects[object_index]);
		}
		object_index++;
	}
}
