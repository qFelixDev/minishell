/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:54:02 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/21 14:52:51 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ms_token		*ms_tokenize(char *string);
t_ms_token		*ms_check_syntax(t_ms_token *token);
t_ms_token		*ms_expand_precedence(t_ms_token *token, int8_t index);
bool			ms_resolve_strings(t_ms_token *token);
t_ms_sequence	*ms_create_sequence(t_ms_token *token);
void			ms_print_sequence(t_ms_sequence *sequence, int indentation);

int	ms_read_delims(t_ms_token *token, t_ms_token *last);
void	ms_traverse_delims(t_ms_sequence *sequence);

t_ms_sequence	*ms_parse(char *string)
{
	t_ms_token			*token;
	t_ms_sequence		*sequence;
	t_ms_token *const	first = ms_tokenize(string);

	if (first == NULL)
		return (printf("Tokenization failed\n"), NULL);
	token = ms_check_syntax(first);
	if (token != NULL)
	{
		gc_close_fd(ms_read_delims(first, token));
		return (printf("Unexpected %s after %s\n",
				ms_get_identity(token->next->index),
				ms_get_identity(token->index)),
			ms_free_tokens(first, false), NULL);
	}
	//ms_print_tokens(first);
	if (ms_expand_precedence(first, MS_TOKEN_AND) == NULL)
		return (printf("Expansion failed\n"),
			ms_free_tokens(first, false), NULL);
	//ms_print_tokens(first);
	if (ms_expand_precedence(first, MS_TOKEN_OR) == NULL)
		return (printf("Expansion failed\n"),
			ms_free_tokens(first, false), NULL);
	//ms_print_tokens(first);
	if (!ms_resolve_strings(first))
		return (printf("Resolution failed\n"),
			ms_free_tokens(first, false), NULL);
	//ms_print_tokens(first);
	sequence = ms_create_sequence(first -> next);
	ms_traverse_delims(sequence);
	ms_free_tokens(first, true);
	return (sequence);
}

/*
t_ms_sequence	*ms_parse(char *string)
{
	t_ms_token			*token;
	t_ms_sequence		*sequence;
	t_ms_token *const	first = ms_tokenize(string);

	if (first == NULL)
		return (printf("Missing quote\n"), NULL);
	token = ms_check_syntax(first);
	if (token != NULL)
		if (token -> next == NULL)
			return (printf("Missing bracket\n"), NULL);
		else
			return (printf("Unexpected %s after %s"),
				ms_get_identity(token -> next -> index),
				ms_get_identity(token -> index));
	ms_expand_precedence(first, MS_TOKEN_AND);
	ms_expand_precedence(first, MS_TOKEN_OR);
	ms_resolve_strings(first);
}

/*bool	print_commands_in_sequence(t_ms_sequence *sequence)
{
	int				object_index;

	object_index = 0;
	while (object_index < (int)sequence->object_count)
	{
		if (sequence -> is_sequence[object_index / 8]
			& (1u << (object_index % 8)))
			print_commands_in_sequence(sequence -> objects[object_index]);
		else
		{
			t_ms_command* command = ms_get_command(sequence->objects[object_index]);
			if (command == NULL)
				return (false);
			ms_print_command(command);
			ms_free_command(command);
		}
		object_index++;
	}
	return (true);
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
		print_commands_in_sequence(sequence);
		ms_free_sequence(sequence);
		ft_memset(buffer, 0, 1000);
	}
}*/