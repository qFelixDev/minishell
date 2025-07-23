/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_2_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:20:00 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 20:20:00 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/wait.h>

void	restore_std_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	gc_close_fd(stdin_backup);
	gc_close_fd(stdout_backup);
}

int	try_execute_builtin(t_ms_command *command, char **build_ins,
				int stdin_backup, int stdout_backup)
{
	int	i;
	int	result;

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(command->argv[0], build_ins[i]) == 0)
		{
			result = buildin_exe(command, i);
			restore_std_fds(stdin_backup, stdout_backup);
			if (result != 127)
				return (result);
		}
		++i;
	}
	return (-1);
}

bool	apply_redirects(char **redirects, int type)
{
	size_t	last_index;
	int		descriptor;
	int		target_fd;

	last_index = 0;
	if (redirects == NULL || redirects[0] == NULL)
		return (true);
	if (!handle_intermediate_redirects(redirects, &last_index))
		return (false);
	target_fd = get_target_fd(type);
	if (!open_redirect_fd(redirects[last_index], type, &descriptor))
		return (false);
	return (dup_and_close(descriptor, target_fd));
}

int	exe_manager(t_ms_command *command, int delim_descriptor)
{
	char	*build_ins[7];
	int		stdin_backup;
	int		stdout_backup;
	int		result;

	if (command == NULL)
		return (1);
	command -> delim_descriptor = delim_descriptor;
	if (command->argv[0] == NULL)
		return (1);
	if (!backup_std_fds(&stdin_backup, &stdout_backup))
		return (perror("dup"), 1);
	if (!apply_redirects(command->redirects[1], 1)
		|| !apply_redirects(command->redirects[2], 2)
		|| !apply_redirects(command->redirects[3], 3))
		return (perror("Redirection Error"), 1);
	generatebuildins(build_ins);
	result = try_execute_builtin(command, build_ins,
			stdin_backup, stdout_backup);
	if (result != -1)
		return (result);
	restore_std_fds(stdin_backup, stdout_backup);
	return (ms_execution_command(command));
}
