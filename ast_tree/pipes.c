/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:09:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/18 16:09:59 by reriebsc         ###   ########.fr       */
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
	ms_exit(tree_monitor(sequence));
}

void	pipe_right_process(t_ms_sequence *sequence, t_pipe_data *pipe_data)
{
	gc_close_fd(pipe_data->pipe_fds[1]);
	dup2(pipe_data->pipe_fds[0], STDIN_FILENO);
	gc_close_fd(pipe_data->pipe_fds[0]);
	pipe_data->right_result = tree_monitor(sequence);
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
	if (!sequence->objects || !sequence->operator + 1)
		perror("No Command du use Pipe");
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
