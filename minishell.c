/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:59:39 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/22 12:13:56 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	handle_shell_input(char *line)
{
	t_ms_command	*command;
	t_ms_sequence	*node;

	command = NULL;
	if(is_empty_str(line))
	{
		gc_free_ptr(line);
		return ;
	}
	node = parse(line);
	if (node == NULL)
	{
		if (ms_minishell_get()->exit_status == 0)
			ms_minishell_get()->exit_status = 1;
		return (gc_free_ptr(line));
	}
	if (!traverse_heredocs(node))
	{
		free_ast_node(node);
		gc_free_ptr(line);
		if (ms_minishell_get()->exit_status == EXIT_SUCCESS)
			ms_minishell_get()->exit_status = EXIT_FAILURE;
		return ;
	}
	return (tree_monitor(node, command), free_ms__sequence(), gc_free_ptr(line));
}

void	minishell_non_interactive_argument(char **args, int argc)
{
	char	*line;

	line = join_str_array(args, argc);
	handle_shell_input(line);
}

void	minishell_interactive(void)
{
	char	*user_prompt;

	user_prompt = NULL;
	main_signals();
	send_greeting();
	while (!ms_minishell_get()->exit_status)
	{
		if (!get_user_prompt_value(&user_prompt))
			break ;
		gc_add(user_prompt);
		handle_shell_input(user_prompt);
	}
}