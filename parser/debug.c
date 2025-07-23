/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:22:05 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/23 16:03:13 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdint.h>

const char	*ms_get_identity(int8_t index)
{
	static char	*token_identities[MS_TOKEN_MAX] = {"&&", "||", "|", "(",
		")", "<<", ">>", "<", ">", "*", "$STR", "\"STR\"", "'STR'", "~STR~"};

	if (index == MS_TOKEN_NONE)
		return ("");
	return (token_identities[index]);
}

void	ms_print_tokens(t_ms_token *token)
{
	while (token != NULL)
	{
		printf("%s ", ms_get_identity(token -> index));
		token = token -> next;
	}
	printf("\n");
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
