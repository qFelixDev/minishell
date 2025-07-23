/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 13:30:02 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 16:35:23 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/wait.h>

char	*ft_find_path(char *cmd)
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

char	*ft_find_exec_path(char **cmd_s, char **env)
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

void	handle_child_process(t_ms_command *command, char **env_cpy)
{
	char		*path;
	struct stat	status;

	if (!apply_redirects(command->redirects[1], 1))
		ms_exit(1);
	if (command->delim_descriptor != -1)
	{
		if (dup2(command->delim_descriptor, STDIN_FILENO) == -1)
			return (perror("minishell: heredoc dup2"), ms_exit(1));
		gc_close_fd(command->delim_descriptor);
		unlink("minishell_delim_file.tmp");
	}
	else if (!apply_redirects(command->redirects[2], 2))
		ms_exit(1);
	if (!apply_redirects(command->redirects[3], 3))
		ms_exit(1);
	if (command -> argv[0] == NULL)
		ms_exit(0);
	path = ft_find_exec_path(command->argv, env_cpy);
	if (lstat(path, &status) != -1 && !S_ISREG(status.st_mode))
		return (ft_putendl_fd("Invalid file mode", 2), ms_exit(126));
	command_signals();
	execve(path, command->argv, env_cpy);
	return (perror("minishell: command not found"), ms_exit(127));
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

bool	backup_std_fds(int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	return (*stdin_backup != -1 && *stdout_backup != -1);
}
