/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_v3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:05:36 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/20 15:12:42 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/wait.h>

int	ms_execution_command(t_ms_command *command)
{
	pid_t	pid;
	char	**env_cpy;
	int		status;

	env_cpy = ms_gen_env();
	if (!env_cpy)
		return (perror(ERROR_MALLOC), 1);
	pid = fork();
	if (pid == -1)
		return (perror(ERROR_FORK), ft_free_cluster(env_cpy), 1);
	if (pid == 0)
	{
		command_signals();
		handle_child_process(command, env_cpy);
	}
	reset_signals();
	waitpid(pid, &status, 0);
	main_signals();
	ft_free_cluster(env_cpy);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}
