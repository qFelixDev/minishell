/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:39:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/13 18:25:54 by ghodges          ###   ########.fr       */
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
		return (ms_exit(0));
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

void exe_manager(t_ms_command *command)
{
	char	*build_ins[7];
	int		i;
	int		result;

	generatebuildins(build_ins);
	i = 0;
	while (i < 7)
	{
		if (ft_strncmp(command->argv[0], build_ins[i], ft_strlen(build_ins[i])) == 0)
		{
			result = buildin_exe(command, i);
			if (result != 127)
				return ;
		}
		++i;
	}
	result = ms_execution_command(command);
}
