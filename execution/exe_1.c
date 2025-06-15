/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:39:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/15 12:44:37 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_find_path(char *cmd, char **env)
{
	int		i;
	char	**path_all;
	char	*sub_path;
	char	*exe_path;

	i = 0;
	path_all = ft_split(ft_getenv("PATH", env), ':');
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

static void	ft_exec_path(char **cmd_s, char **env)
{
	if (access(cmd_s[0], X_OK) != 0)
	{
		ft_putstr_fd("pipex: ", 2);
		perror(cmd_s[0]);
		ft_free_cluster(cmd_s);
		exit(126);
	}
	execve(cmd_s[0], cmd_s, env);
	perror(cmd_s[0]);
	ft_free_cluster(cmd_s);
	exit(127);
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
		path = ft_find_path(cmd_s[0], env);
	return (path);
}

static void	ft_handle_invalid_cmd(char **cmd_s)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(": command not found\n", 2);
	if (cmd_s)
		ft_free_cluster(cmd_s);
	exit(127);
}

void	ft_process(char *cmd, char **env)
{
	pid_t	pid;
	int		p_fd[2];

	if (pipe(p_fd) == -1)
		perror("pipe");
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		close(p_fd[0]);
		dup2(p_fd[1], STDOUT_FILENO);
		ft_execute_cmd(cmd, env);
	}
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}
