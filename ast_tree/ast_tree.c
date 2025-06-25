/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 14:36:19 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/25 12:09:14 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



void	and_monitor(t_ms_sequence *sequence)
{
	size_t			i;
	t_ms_command	*command;

	i = 0;
	while (i < sequence->object_count)
	{
		if (sequence->is_sequence[i / 8] & (1 << (i % 8)))
			tree_monitor(sequence);
		else
		{
			command = ms_get_command(sequence->objects);
			exe_manager(command);
		}
		++i;
	}
}

void	or_monitor(t_ms_sequence *sequence)
{
	
}

// Objekt ist eine Unter-Sequenz → rekursiver Aufruf
// Objekt ist ein Kommando
int	tree_monitor(t_ms_sequence *sequence)
{
	size_t			i;
	t_ms_command	*command;

	i = 0;
	if (!sequence)
		return (0);
	while (i < sequence->object_count)
	{
		if (sequence->is_sequence[i / 8] & (1 << (i % 8)))
		{
			if (sequence->operator == MS_TOKEN_PIPE)
				pipe_monitor(sequence);
			else if (sequence->operator == MS_TOKEN_AND)
				and_monitor(sequence);
			else if (sequence->operator == MS_TOKEN_OR)
				or_monitor(sequence);
		}
		else
		{ 
			command = ms_get_command(sequence->objects);
			exe_manager(command);
		}
		++i;
	}
	return (0);
}


//else
//{
//	t_ms_command *cmd = (t_ms_command *)sequence->objects[i];
//	execute_command(cmd); // Beispiel: übergib Kontext o.ä.
//}

//tree_monitor((t_ms_sequence *)sequence->objects[i]);