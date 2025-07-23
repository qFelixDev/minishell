/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:39:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 16:43:16 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ms_execution_command(t_ms_command *command);

void	generatebuildins(char *buildins[7])
{
	buildins[0] = "env";
	buildins[1] = "exit";
	buildins[2] = "unset";
	buildins[3] = "export";
	buildins[4] = "pwd";
	buildins[5] = "cd";
	buildins[6] = "echo";
}

int	buildin_exe(t_ms_command *command, int index_buildin)
{
	int	res;

	res = 1;
	(void)res;
	if (index_buildin == 0)
		return (ms_print_env());
	else if (index_buildin == 1)
		return (ms_exit_builtin(command));
	else if (index_buildin == 2)
		return (ms_unset(command));
	else if (index_buildin == 3)
		return (ms_export(command -> argv));
	else if (index_buildin == 4)
		return (ms_pwd());
	else if (index_buildin == 5)
		return (ms_cd(command->argv[1]));
	else if (index_buildin == 6)
		return (ms_echo(command, false));
	return (127);
}

int	ms_execute_sequence(t_ms_sequence *sequence)
{
	size_t			obj_idx;
	int				exit_code;
	t_ms_command	*command;
	const bool		expectation = (sequence -> operator == MS_TOKEN_AND);

	if (sequence -> operator == MS_TOKEN_PIPE)
		return (pipe_monitor(sequence));
	obj_idx = 0;
	while (obj_idx < sequence -> object_count)
	{
		if (sequence -> is_sequence[obj_idx / 8]
			& (1u << (obj_idx % 8)))
			exit_code = ms_execute_sequence(sequence->objects[obj_idx]);
		else
		{
			command = ms_get_command(sequence -> objects[obj_idx]);
			exit_code = exe_manager(command, sequence -> delim_descriptors[obj_idx]);
			ms_free_command(command);
		}
		if (!exit_code != expectation)
			return (ms_minishell_get()->exit_status = exit_code);
		obj_idx++;
	}
	return (ms_minishell_get()->exit_status = exit_code, exit_code);
}
