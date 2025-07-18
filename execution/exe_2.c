/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 13:30:02 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/18 21:58:57 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/wait.h>

//if (sequence->is_sequence[object_index / 8] & (1u << (object_index % 8)))
// if (sequence->is_sequence[i / 8] & (1u << (i % 8)))

static char	*ft_find_path(char *cmd)
{
	int		i;
	char	**path_all;
	char	*sub_path;
	char	*exe_path;

	if (ft_strchr(cmd, '/') != NULL)
		return (ft_strdup(cmd));
	i = 0;
	path_all = ft_split(ft_getenv("PATH"), ':');
	while (path_all[i])
	{
		sub_path = ft_strjoin(path_all[i], "/");
		exe_path = ft_strjoin(sub_path, cmd);
		free(sub_path);
		if (access(exe_path, F_OK | X_OK) == 0)
		{
			ft_free_cluster(path_all);
			return (exe_path);
		}
		free(exe_path);
		i++;
	}
	return (ft_free_cluster(path_all), NULL);
}

static char	*ft_find_exec_path(char **cmd_s, char **env)
{
	char	*path;

	if (!env || !*env)
	{
		path = ft_strjoin("/bin/", cmd_s[0]);
		if (access(path, X_OK) == -1)
		{
			free(path);
			path = ft_strjoin("/usr/bin/", cmd_s[0]);
		}
	}
	else
		path = ft_find_path(cmd_s[0]);
	return (path);
}

static void	handle_child_process(t_ms_command *command, char **env_cpy)
{
	char *path;

	path = ft_find_exec_path(command->argv, env_cpy);
	command_signals();
	execve(path, command->argv, env_cpy);
	//command_not_found(path);
	/*if (ms_minishell_get()->or_sequenze)
		ms_minishell_get()->finish_or = false;
	if (!ms_minishell_get()->or_sequenze)
		ms_exit(EXIT_FAILURE);*/
	ms_exit(EXIT_FAILURE);
}

int	wait_for_process(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		printf("\n");
		return (128 + WTERMSIG(status));
	}
	return (WEXITSTATUS(status));
}

int	ms_execution_command(t_ms_command *command)
{
	pid_t	pid;
	char	**env_cpy;

	env_cpy = ms_gen_env();
	if (!env_cpy)
		return (perror(ERROR_MALLOC), 1);
	pid = fork();
	if (pid == -1)
		return (perror(ERROR_FORK), 1);
	if (pid == 0)
		handle_child_process(command, env_cpy);
	return (main_signals(), ft_free_cluster(env_cpy), wait_for_process(pid));
}

