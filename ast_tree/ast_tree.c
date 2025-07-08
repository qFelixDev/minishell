/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 14:36:19 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/07 10:37:51 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void exe_manager(t_ms_command *command);

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
			command = ms_get_command(sequence->objects[i]);
			exe_manager(command);
		}
		++i;
	}
}

const char* ms_get_identity(int8_t index);

void	or_monitor(t_ms_sequence *sequence)
{
	size_t			i;
	t_ms_command	*command;

	i = 0;
	while (i < sequence->object_count && !ms_minishell_get()->finish_or)
	{
		ms_minishell_get()->finish_or = true;
		ms_minishell_get()->or_sequenze = true;
		if (sequence->is_sequence[i / 8] & (1 << (i % 8)))
			tree_monitor(sequence -> objects[i]);
		else
		{
			command = ms_get_command(sequence->objects[i]);
			exe_manager(command);
		}
		++i;
	}
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
				pipe_monitor(sequence->objects[i]);
			else if (sequence->operator == MS_TOKEN_AND)
				and_monitor(sequence->objects[i]);
			else if (sequence->operator == MS_TOKEN_OR) {
				or_monitor(sequence->objects[i]);
			}
		}
		else
		{
			command = ms_get_command(sequence->objects[i]);
			exe_manager(command);
		}
		++i;
	}
	return (0);
}

/*
static bool	process_heredoc(t_list *redirects, t_ms_sequence *sequence)
{
	t_list		*current;
	t_redirect	*redirect;

	current = redirects;
	while (current)
	{
		redirect = (t_redirect *)current->content;
		if (redirect->type == MS_TOKEN_REDIRECT_INPUT_APPEND)
		{
			if (!redirection_heredoc(redirect->file, sequence))
				return (false);
		}
		current = current->next;
	}
	return (true);
}

bool	traverse_heredocs(t_ms_sequence *sequence)
{
//	t_list	*redirects;
	int		exit_code;
	char	*tmp;
	bool	result;

	if (sequence->is_sequence[0] & 1)
	{
		if (!traverse_heredocs(sequence->left))
			return (false);
		if (!traverse_heredocs(sequence->right))
			return (false);
		//comming part is maby not useful
		//if (!(node->type == AST_PARENTHESES && node->value))
		//	return (true);
	}
//	redirects = NULL;
	//Not Useful the next part for our projekt 
	tmp = filter_and_get_redirects(ms_get_command(sequence->objects[0]), &redirects, &exit_code);
	//if (!handle_parenteses(tmp, node))
	//	return (gc_free_ptr(tmp), gc_list_clear(&redirects, free_redirect),
	//		false);
	//gc_free_ptr(tmp);
	result = process_heredoc(redirects, sequence);
	gc_list_clear(&redirects, free_redirect);
	return (result);
}
*/