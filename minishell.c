/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:59:39 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 15:48:03 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	handle_shell_input(char *line)
{
	t_ms_sequence	*sequence;

	if (line[0] == '\0')
		return ;
	add_history(line);
	sequence = ms_parse(line);
	if (sequence == NULL)
	{
		ms_minishell_get()->exit_status = 258;
		return (gc_free_ptr(line));
	}
	return (ms_execute_sequence(sequence), ms_free_sequence(sequence),
		gc_free_ptr(line));
}

void	minishell_non_interactive_2(void)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (line[0] == '\n' || line[0] == '\0')
		{
			gc_free_ptr(line);
			continue ;
		}
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		gc_add(line);
		handle_shell_input(line);
		line = get_next_line(STDIN_FILENO);
	}
}

void	non_interactive_arg(char **args, int argc)
{
	char	*line;

	line = join_str_array(args, argc);
	write(STDIN_FILENO, line, ft_strlen(line));
	write(STDIN_FILENO, "\n", 1);
	gc_free_ptr(line);
	minishell_non_interactive_2();
}

void	interactive(void)
{
	char	*user_prompt;
	int		tty;

	tty = gc_add_fd(open("/dev/tty", O_WRONLY));
	user_prompt = NULL;
	main_signals();
	while (get_user_prompt_value(&user_prompt, tty))
	{
		gc_add(user_prompt);
		handle_shell_input(user_prompt);
	}
	gc_close_fd(tty);
}

void	minishell_non_interactive(void)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		gc_add(line);
		if (*line == '\0' || *line == '\n')
		{
			gc_free_ptr(line);
			continue ;
		}
		handle_shell_input(line);
		line = get_next_line(STDIN_FILENO);
	}
}
