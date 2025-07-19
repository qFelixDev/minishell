/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:39:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/19 17:04:37 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ms_execution_command(t_ms_command *command);

static void	generatebuildins(char *buildins[7])
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
		return (ms_exit(ms_minishell_get()->exit_status));
	else if (index_buildin == 2)
		return (ms_unset(command));
	else if (index_buildin == 3)
		return (ms_export(command));
	else if (index_buildin == 4)
		return (ms_pwd());
	else if (index_buildin == 5)
		return (ms_cd(command->argv[1]));
	else if (index_buildin == 6)
		return (ms_echo(command, false));
	return (127);
}

int	exe_manager(t_ms_command *command)
{
	char				*build_ins[7];
	int					i;
	int					result;

	if (command == NULL)
		return (1);
	generatebuildins(build_ins);
	i = 0;
	while (i < 7)
	{
		if (ft_strncmp(command->argv[0], build_ins[i],
				ft_strlen(command -> argv[0])) == 0)
		{
			result = buildin_exe(command, i);
			if (result != 127)
				return (result);
		}
		++i;
	}
	return (ms_execution_command(command));
}

int	ms_execute_sequence(t_ms_sequence *sequence)
{
	size_t			object_index;
	int				exit_code;
	t_ms_command	*command;
	const bool		expectation = (sequence -> operator == MS_TOKEN_AND);

	if (sequence -> operator == MS_TOKEN_PIPE)
		return (pipe_monitor(sequence));
	object_index = 0;
	while (object_index < sequence -> object_count)
	{
		if (sequence -> is_sequence[object_index / 8]
			& (1u << (object_index % 8)))
			exit_code = ms_execute_sequence(sequence->objects[object_index]);
		else
		{
			command = ms_get_command(sequence -> objects[object_index]);
			exit_code = exe_manager(command);
			ms_free_command(command);
		}
		if (!exit_code != expectation)
			return (ms_minishell_get()->exit_status = exit_code);
		object_index++;
	}
	ms_minishell_get()->exit_status = exit_code;
	return (exit_code);
}
