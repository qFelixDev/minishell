/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:09:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/18 22:00:01 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>

void	pipe_left_process(t_ms_sequence *sequence, t_pipe_data *pipe_data)
{
	gc_close_fd(pipe_data->pipe_fds[0]);
	dup2(pipe_data->pipe_fds[1], STDOUT_FILENO);
	gc_close_fd(pipe_data->pipe_fds[1]);
	ms_exit(ms_execute_sequence(sequence));
}

void	pipe_right_process(t_ms_sequence *sequence, t_pipe_data *pipe_data)
{
	gc_close_fd(pipe_data->pipe_fds[1]);
	dup2(pipe_data->pipe_fds[0], STDIN_FILENO);
	gc_close_fd(pipe_data->pipe_fds[0]);
	pipe_data->right_result = ms_execute_sequence(sequence);
	ms_exit(pipe_data->right_result);
}

void	pipe_fork_error(t_pipe_data *pipe_data)
{
	kill(pipe_data->left_pid, SIGKILL);
	gc_close_fd(pipe_data->pipe_fds[0]);
	gc_close_fd(pipe_data->pipe_fds[1]);
	return (perror("Fork for right process failed"));
}

void	close_pipe_and_wait(t_pipe_data *pipe_data)
{
	gc_close_fd(pipe_data->pipe_fds[0]);
	gc_close_fd(pipe_data->pipe_fds[1]);
	waitpid(pipe_data->left_pid, NULL, 0);
	waitpid(pipe_data->right_pid, &pipe_data->right_status, 0);
	main_signals();
}

void	pipe_monitor(t_ms_sequence *sequence)
{
	t_pipe_data	pipe_data;

	reset_signals();
	if (!sequence->objects) //|| !sequence->operator + 1)
		return (perror("No Command du use Pipe"));
	if (pipe(pipe_data.pipe_fds) == -1)
		return (perror("Pipe creation failed"));
	pipe_data.left_pid = fork();
	if (pipe_data.left_pid == -1)
		return (perror("Fork for left process failed"));
	if (pipe_data.left_pid == 0)
		pipe_left_process(sequence, &pipe_data);
	pipe_data.right_pid = fork();
	if (pipe_data.right_pid == -1)
		return (pipe_fork_error(&pipe_data));
	if (pipe_data.right_pid == 0)
		pipe_right_process(sequence, &pipe_data);
	close_pipe_and_wait(&pipe_data);
	if (WIFEXITED(pipe_data.right_status))
		pipe_data.right_result = WEXITSTATUS(pipe_data.right_status);
	ms_minishell_get()->exit_status = pipe_data.right_result;
	//return (pipe_data.right_result);
}

int	ft_process(t_ms_command *command)
{
	pid_t	pid;
	int		p_fd[2];
	int		res;

	if (pipe(p_fd) == -1)
		perror("pipe");
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		gc_close_fd(p_fd[0]);
		dup2(p_fd[1], STDOUT_FILENO);
		res = exe_manager(command);
		ms_free_command(command);
		return (res);
	}
	else
	{
		gc_close_fd(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
		ms_free_command(command);
	}
}


int	pipe_monitor(t_ms_sequence *sequence)
{
	size_t			pipe_index;
	t_ms_command	*command;

	pipe_index = 0;
	while (pipe_index < sequence -> object_count - 1)
	{
		if (sequence -> is_sequence[pipe_index / 8]
			& (1u << (pipe_index % 8)))
		if (!(sequence -> is_sequence[pipe_index / 8]
			& (1u << (pipe_index % 8))))
			ft_process(ms_get_command(sequence->objects[pipe_index]));

		pipe_index++;
	}
}


#include "pipex_bonus.h"

static void	ft_here_doc_input(char *limiter, int write_fd)
{
	char	*line;

	line = get_next_line(0);
	while (line)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		write(write_fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	exit(EXIT_SUCCESS);
}
//
//void	ft_here_doc(char *limiter, int argc)
//{
//	pid_t	pid;
//	int		fd[2];
//
//	if (argc < 6)
//		exit(127);
//	if (pipe(fd) == -1)
//		perror("pipe");
//	pid = fork();
//	if (pid == -1)
//		perror("fork");
//	if (pid == 0)
//	{
//		close(fd[0]);
//		ft_here_doc_input(limiter, fd[1]);
//	}
//	else
//	{
//		close(fd[1]);
//		dup2(fd[0], STDIN_FILENO);
//		wait(NULL);
//	}
//}
//
//void	ft_process(char *cmd, char **env)
//{
//	pid_t	pid;
//	int		p_fd[2];
//
//	if (pipe(p_fd) == -1)
//		perror("pipe");
//	pid = fork();
//	if (pid == -1)
//		perror("fork");
//	if (pid == 0)
//	{
//		close(p_fd[0]);
//		dup2(p_fd[1], STDOUT_FILENO);
//		ft_execute_cmd(cmd, env);
//	}
//	else
//	{
//		close(p_fd[1]);
//		dup2(p_fd[0], STDIN_FILENO);
//		waitpid(pid, NULL, 0);
//	}
//}
//
//int	main(int ac, char **av, char **env)
//{
//	int		x;
//	int		fd_in;
//	int		fd_out;
//
//	if (ac >= 5)
//	{
//		if (ft_strncmp(av[1], "here_doc", 8) == 0)
//		{
//			fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
//			ft_here_doc(av[2], ac);
//			x = 3;
//		}
//		else
//		{
//			fd_in = open(av[1], O_RDONLY, 0777);
//			fd_out = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
//			dup2(fd_in, STDIN_FILENO);
//			x = 2;
//		}
//		while (x < ac - 2)
//			ft_process(av[x++], env);
//		dup2(fd_out, STDOUT_FILENO);
//		ft_execute_cmd(av[ac - 2], env);
//	}
//}
