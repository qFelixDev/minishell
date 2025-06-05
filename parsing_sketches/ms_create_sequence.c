/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_create_sequence.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:52:06 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/05 11:10:39 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

size_t	count_objects(t_ms_token *token)
{
	size_t	object_count;
	int		bracket_level;

	object_count = 1;
	bracket_level = 0;
	token = token -> next;
	while (bracket_level >= 0)
	{
		if (bracket_level == 0 && (token -> index == MS_TOKEN_AND
				|| token -> index == MS_TOKEN_OR
				|| token -> index == MS_TOKEN_PIPE))
			object_count++;
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = token -> next;
	}
	return (object_count);
}

t_ms_command	*ms_create_command(t_ms_token *first)
{
	t_ms_token			*token;
	size_t				redirect_counts[4];
	size_t				redirect_indices[4];
	t_ms_command *const	command = malloc(sizeof(t_ms_command));

	token = first;
	while (token -> index >= MS_TOKEN_STRING_OPAQUE
		|| (token->index >= MS_TOKEN_DELIM && token->index <= MS_TOKEN_OUTPUT))
	{
		redirect_counts[0] += (token -> index == MS_TOKEN_DELIM);
		redirect_counts[1] += (token -> index == MS_TOKEN_APPEND);
		redirect_counts[2] += (token -> index == MS_TOKEN_INPUT);
		redirect_counts[3] += (token -> index == MS_TOKEN_OUTPUT);
		token = token -> next;
	}
	command->delimiter_redirects = malloc(sizeof(char*) * redirect_counts[0]);
	command->append_redirects = malloc(sizeof(char*) * redirect_counts[1]);
	command->input_redirects = malloc(sizeof(char*) * redirect_counts[2]);
	command->output_redirects = malloc(sizeof(char*) * redirect_counts[3]);
}

t_ms_command	*ms_create_command(t_ms_token *first)
{
	t_ms_token			*token;
	int					argc;
	int					arg_index;
	size_t				length;
	t_ms_command *const	command = malloc(sizeof(t_ms_command));

	if (command == NULL)
		return (NULL);
	argc = 1;
	while (token -> index >= MS_TOKEN_STRING_OPAQUE)
	{
		argc += !token -> concatenate_content;
		token = token -> next;
	}
	command -> argv = ft_calloc(sizeof(char*) * (argc + 1), 1);
	if (command -> argv == NULL)
		return (free(command), NULL);
	arg_index = 0;
	while (arg_index < argc)
	{
		token = 
		if (command -> argv[arg_index++] == NULL)
			return (ms_free_command(command), NULL);
	}
	return (command);
}

void	ms_free_command(t_ms_command *command)
{
	char	**argument;

	argument = command -> argv;
	while (*argument != NULL)
		free(*(argument++));
	free(command -> argv);
	//free(command -> redirect_delim);
	//free(command -> redirect_append);
	//free(command -> redirect_input);
	//free(command -> redirect_output);
	free(command);
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
				ms_free_command(sequence -> objects[object_index]);
		}
		object_index++;
	}
	free(sequence);
}

t_ms_sequence	*ms_create_sequence(t_ms_token **token_pt)
{
	size_t					object_index;
	t_ms_sequence *const	sequence = malloc(sizeof(t_ms_sequence));

	sequence -> object_count = count_objects(*token_pt);
	sequence -> objects = ft_calloc(sizeof(void*) * sequence->object_count, 1);
	sequence -> is_sequence
		= ft_calloc((sizeof(uint8_t) * sequence -> object_count + 1) / 8, 1);
	if (sequence -> objects == NULL || sequence -> is_sequence == NULL)
		return (free(sequence->objects), free(sequence->is_sequence), NULL);
	object_index = 0;
	*token_pt = (*token_pt) -> next;
	while ((*token_pt) -> index != MS_TOKEN_CLOSE)
	{
		if ((*token_pt) -> index >= MS_TOKEN_STRING_OPAQUE)
			sequence -> objects[object_index++] = ms_create_command(*token_pt);
		if ((*token_pt) -> index == MS_TOKEN_OPEN)
			sequence -> objects[object_index++] = ms_create_sequence(token_pt);
		if ((*token_pt) -> index >= MS_TOKEN_STRING_OPAQUE
			|| (*token_pt) -> index == MS_TOKEN_OPEN
			&& sequence -> objects[object_index - 1] == NULL)
			return (ms_free_sequence(sequence), NULL);
		sequence -> is_sequence[object_index / 8] |= (1u << (object_index % 8))
				* ((*token_pt) -> index == MS_TOKEN_OPEN);
		*token_pt = (*token_pt) -> next;
	}
	*token_pt = (*token_pt) -> next;
	return (sequence);
}
