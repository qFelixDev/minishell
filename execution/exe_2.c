/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 13:30:02 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/19 15:58:38 by ghodges          ###   ########.fr       */
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

static bool	apply_redirects(char **redirects, int type)
{
	size_t	redirect_index;
	size_t	last_index;
	int		descriptor;

	if (redirects[0] == NULL)
		return (true);
	last_index = 0;
	while (redirects[last_index + 1] != NULL)
		last_index++;
	redirect_index = 0;
	while (redirect_index < last_index)
	{
		descriptor = open(redirects[redirect_index++],
			O_CREAT * (type != 2), 0777);
		if (descriptor == -1)
			return (false);
		close(descriptor);
	}
	descriptor = open(redirects[last_index], (O_CREAT * (type != 2))
		| (O_WRONLY * (type != 2)) | (O_RDONLY * (type == 2))
		| (O_APPEND * (type == 1)) | (O_TRUNC * (type == 3)), 0777);
	if (descriptor == -1)
		return (false);
	dup2(descriptor, STDOUT_FILENO * (type != 2) + STDIN_FILENO * (type == 2));
	return (close(descriptor), true);
}

static void	handle_child_process(t_ms_command *command, char **env_cpy)
{
	char	*path;

	if (!apply_redirects(command -> redirects[1], 1))
		ms_exit(1);
	if (!apply_redirects(command -> redirects[2], 2))
		ms_exit(1);
	if (!apply_redirects(command -> redirects[3], 3))
		ms_exit(1);
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



//int	ms_execution_command(t_ms_command *command)
//{
//	//pid_t	pid;
//	char	**env_cpy;
//
//	env_cpy = ms_gen_env();
//	if (!env_cpy)
//		return (perror(ERROR_MALLOC), 1);
//	//pid = fork();
//	//if (pid == -1)
//	//	return (perror(ERROR_FORK), 1);
//	//if (pid == 0)
//	handle_child_process(command, env_cpy);
//	return (main_signals(), ft_free_cluster(env_cpy), 1); //wait_for_process(pid));
//}


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
		handle_child_process(command, env_cpy);

	// Parent: wartet auf das Kind und räumt auf
	waitpid(pid, &status, 0);
	main_signals(); // ggf. Signale wieder setzen
	ft_free_cluster(env_cpy);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1); // Abnormaler Exit (z.B. durch Signal)
}

