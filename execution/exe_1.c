/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:39:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/05 13:05:27 by ghodges          ###   ########.fr       */
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
	if (index_buildin == 0)
		ms_print_env();
	//else if (index_buildin == 1)
	//	ms_exit();
	//else if (index_buildin == 2)
	//	ms_unset();
	//else if (index_buildin == 3)
	//	ms_export();
	else if (index_buildin == 4)
		ms_pwd();
	else if (index_buildin == 5)
		ms_cd(command->argv[1]);
	//else if (index_buildin == 6)
	//	ms_echo();
	return (0);
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
		if (ft_strncmp(command->argv[0], build_ins[i], ft_strlen(build_ins[i]) + 1) == 0)
		{
			result = buildin_exe(command, i);
		}
		++i;
	}
	result = ms_execution_command(command);
}
