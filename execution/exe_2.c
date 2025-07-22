/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 13:30:02 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/22 18:07:21 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/wait.h>

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

//static bool	apply_redirects(char **redirects, int type)
//{
//	size_t	redirect_index;
//	size_t	last_index;
//	int		descriptor;
//
//	if (redirects[0] == NULL)
//		return (true);
//	last_index = 0;
//	while (redirects[last_index + 1] != NULL)
//		last_index++;
//	redirect_index = 0;
//	while (redirect_index < last_index)
//	{
//		descriptor = open(redirects[redirect_index++],
//				O_CREAT * (type != 2), 0777);
//		if (descriptor == -1)
//			return (false);
//		close(descriptor);
//	}
//	descriptor = open(redirects[last_index], (O_CREAT * (type != 2))
//			| (O_WRONLY * (type != 2)) | (O_RDONLY * (type == 2))
//			| (O_APPEND * (type == 1)) | (O_TRUNC * (type == 3)), 0777);
//	if (descriptor == -1)
//		return (false);
//	dup2(descriptor, STDOUT_FILENO * (type != 2) + STDIN_FILENO * (type == 2));
//	return (close(descriptor), true);
//}

static bool	apply_redirects(char **redirects, int type)
{
	size_t	redirect_index = 0;
	size_t	last_index = 0;
	int		descriptor;
	int		target_fd;

	if (redirects == NULL || redirects[0] == NULL)
		return (true);

	// Bestimme Ziel-FD
	if (type == 2)
		target_fd = STDIN_FILENO;
	else
		target_fd = STDOUT_FILENO;

	// Finde das letzte Redirect-Target
	while (redirects[last_index + 1] != NULL)
		last_index++;

	// Erstelle leere Dateien für alles außer das letzte
	while (redirect_index < last_index)
	{
		descriptor = open(redirects[redirect_index++], O_CREAT | O_WRONLY, 0644);
		if (descriptor == -1)
			return (perror("open (dummy redirect)"), false);
		close(descriptor);
	}

	// Öffne die eigentliche Datei für die Redirection
	if (type == 1) // >>
		descriptor = open(redirects[last_index], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (type == 2) // <
		descriptor = open(redirects[last_index], O_RDONLY);
	else if (type == 3) // >
		descriptor = open(redirects[last_index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		return (false);

	if (descriptor == -1)
		return (perror("open (redirect)"), false);

	if (dup2(descriptor, target_fd) == -1)
		return (perror("dup2 (redirect)"), close(descriptor), false);

	close(descriptor);
	return (true);
}


void	handle_child_process(t_ms_command *command, char **env_cpy)
{
	char	*path;

	// 1. Apply >> (append) redirects
	if (!apply_redirects(command->redirects[1], 1))
		ms_exit(1);

	// 2. Apply << heredoc or < infile
	if (command->delim_descriptor != -1)
	{
		if (dup2(command->delim_descriptor, STDIN_FILENO) == -1)
		{
			perror("minishell: heredoc dup2");
			ms_exit(1);
		}
		gc_close_fd(command->delim_descriptor);
		unlink("minishell_delim_file.tmp");
	}
	else
	{
		if (!apply_redirects(command->redirects[2], 2)) // only if no heredoc
			ms_exit(1);
	}

	// 3. Apply > (overwrite) redirects
	if (!apply_redirects(command->redirects[3], 3))
		ms_exit(1);

	// 4. Path + exec
	path = ft_find_exec_path(command->argv, env_cpy);
	command_signals();
	execve(path, command->argv, env_cpy);

	// 5. Fehlerbehandlung falls execve fehlschlägt
	perror("minishell: command not found");
	ms_exit(127);
}


//void	handle_child_process(t_ms_command *command, char **env_cpy)
//{
//	char	*path;
//
//	if (!apply_redirects(command -> redirects[1], 1))
//		ms_exit(1);
//	if (!apply_redirects(command -> redirects[2], 2))
//		ms_exit(1);
//	if (!apply_redirects(command -> redirects[3], 3))
//		ms_exit(1);
//	path = ft_find_exec_path(command->argv, env_cpy);
//	command_signals();
//	if (command -> delim_descriptor != -1)
//	{
//		dup2(command -> delim_descriptor, STDIN_FILENO);
//		gc_close_fd(command -> delim_descriptor);
//		unlink("minishell_delim_file.tmp");
//	}
//	execve(path, command->argv, env_cpy);
//	perror("Command not Found");
//	ms_exit(127);
//}

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

int	exe_manager(t_ms_command *command)
{
	char	*build_ins[7];
	int		i;
	int		result;
	int		stdin_backup;
	int		stdout_backup;

	if (command->argv[0] == NULL) 
		return (1);

	// === Backup Original-FDs ===
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

	// === Redirections anwenden ===
	if (!apply_redirects(command->redirects[1],  1)) // >>
		return (perror("Redirection Error"), 1);
	if (!apply_redirects(command->redirects[2], 2)) // <
		return (perror("Redirection Error"), 1);
	if (!apply_redirects(command->redirects[3], 3)) // >
		return (perror("Redirection Error"), 1);

	generatebuildins(build_ins);
	i = 0;
	while (i < 7)
	{
		if (ft_strncmp(command->argv[0], build_ins[i],
				ft_strlen(command->argv[0])) == 0)
		{
			result = buildin_exe(command, i);

			// === Restore FDs ===
			dup2(stdin_backup, STDIN_FILENO);
			dup2(stdout_backup, STDOUT_FILENO);
			close(stdin_backup);
			close(stdout_backup);

			if (result != 127)
				return (result);
		}
		++i;
	}

	// === Restore FDs vor externem Kommando ===
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);

	return (ms_execution_command(command));
}
