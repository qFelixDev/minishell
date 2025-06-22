/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 14:36:19 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/22 14:55:51 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Objekt ist eine Unter-Sequenz → rekursiver Aufruf
// Objekt ist ein Kommando
int	tree_monitor(t_ms_sequence *sequence)
{
	size_t	i;

	i = 0;
	if (!sequence)
		return (0);
	while (i < sequence->object_count)
	{
		if (sequence->is_sequence[i / 8] & (1 << (i % 8)))
			tree_monitor((t_ms_sequence *)sequence->objects[i]);
		else
		{
			t_ms_command *cmd = (t_ms_command *)sequence->objects[i];
			execute_command(cmd, command); // Beispiel: übergib Kontext o.ä.
		}
		// Nach jedem Objekt: je nach Operator reagieren
		if (sequence->operator == MS_TOKEN_PIPE)
			pipe_monitor(sequence);
		else if (sequence->operator == MS_TOKEN_AND)
			and_monitor(sequence, command);
		else if (sequence->operator == MS_TOKEN_OR)
			or_monitor(sequence, command);
		else if (sequence->operator == MS_TOKEN_OPEN)  // subshell
			parentheses_monitor(sequence, command);
		++i;
	}
	return (0);
}
