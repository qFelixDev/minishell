/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:09:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/18 21:54:53 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>

int	exe_manager(t_ms_command *command);

/*void	pipe_left_process(t_ms_sequence *sequence, t_pipe_data *pipe_data)
{
	t_ms_command	*command;

	gc_close_fd(pipe_data->pipe_fds[0]);
	dup2(pipe_data->pipe_fds[1], STDOUT_FILENO);
	gc_close_fd(pipe_data->pipe_fds[1]);
	if (sequence -> is_sequence[0] & 1)
		ms_exit(ms_execute_sequence(sequence -> objects[0]));
	command = ms_get_command(sequence -> objects[0]);
	ms_exit(exe_manager(command));
}

void	pipe_right_process(t_ms_sequence *sequence, t_pipe_data *pipe_data)
{
	t_ms_command	*command;

	gc_close_fd(pipe_data->pipe_fds[1]);
	dup2(pipe_data->pipe_fds[0], STDIN_FILENO);
	gc_close_fd(pipe_data->pipe_fds[0]);
	if (sequence -> is_sequence[0] & 2)
		ms_exit(ms_execute_sequence(sequence));
	command = ms_get_command(sequence -> objects[1]);
	//pipe_data->right_result = ms_execute_sequence(sequence);
	ms_exit(exe_manager(command));
}*/

void	pipe_process(
	t_ms_sequence *sequence, size_t object_index, int *descriptors,
	int std_descriptor)
{
	gc_close_fd(descriptors[0]);
	dup2(descriptors[1], std_descriptor);
	gc_close_fd(descriptors[1]);
	if (sequence -> is_sequence[object_index / 8]
		& (1u << (object_index % 8)))
		ms_exit(ms_execute_sequence(sequence));
	ms_exit(exe_manager(ms_get_command(sequence -> objects[object_index])));
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

int	ms_execute_pipe(t_ms_sequence *sequence, size_t object_index)
{
	t_pipe_data	pipe_data;

	reset_signals();
	if (pipe(pipe_data.pipe_fds) == -1)
		return (perror("Pipe creation failed"), 1);
	pipe_data.left_pid = fork();
	if (pipe_data.left_pid == -1)
		return (perror("Fork for left process failed"), 1);
	if (pipe_data.left_pid == 0)
		//pipe_left_process(sequence, &pipe_data);
		pipe_process(sequence, object_index, (int[2]) {pipe_data.pipe_fds[0], pipe_data.pipe_fds[1]}, STDOUT_FILENO);
	pipe_data.right_pid = fork();
	if (pipe_data.right_pid == -1)
		return (pipe_fork_error(&pipe_data), 1);
	if (pipe_data.right_pid == 0)
		pipe_process(sequence, object_index + 1, (int[2]) {pipe_data.pipe_fds[1], pipe_data.pipe_fds[0]}, STDIN_FILENO);
	close_pipe_and_wait(&pipe_data);
	if (WIFEXITED(pipe_data.right_status))
		pipe_data.right_result = WEXITSTATUS(pipe_data.right_status);
	ms_minishell_get()->exit_status = pipe_data.right_result;
	//printf("%d\n", ms_minishell_get() -> exit_status);
	//return (pipe_data.right_result);
	return (0);
}

int	pipe_monitor(t_ms_sequence *sequence)
{
	size_t	object_index;
	int		exit_code;

	object_index = 0;
	while (object_index < sequence -> object_count - 1)
	{
		exit_code = ms_execute_pipe(sequence, object_index);
		object_index++;
	}
	return (exit_code);
}

void	ms_execute_pipeline_element(
	t_ms_sequence *sequence, size_t object_index, int *pipes, size_t pipe_count)
{
	t_ms_command	*command;

	reset_signals();
	if (object_index != 0)
		if (dup2(pipes[(object_index - 1) * 2], STDIN_FILENO) == -1)
			ms_exit(1);	
	if (object_index != pipe_count)
		if (dup2(pipes[object_index * 2 + 1], STDOUT_FILENO) == -1)
			ms_exit(1);
	if (object_index != 0)
		close(pipes[(object_index - 1) * 2]);
	if (object_index != pipe_count)
		close(pipes[object_index * 2 + 1]);
	if (sequence -> is_sequence[object_index / 8]
		& (1u << (object_index % 8)))
		ms_exit(ms_execute_sequence(sequence -> objects[object_index]));
	command = ms_get_command(sequence -> objects[object_index]);
	int exit_code = exe_manager(command);
	ms_free_command(command);
	ms_exit(exit_code);
}

int	ms_execute_pipeline(t_ms_sequence *sequence)
{
	size_t			object_index;
	const size_t	pipe_count = sequence -> object_count - 1;
	pid_t *const	process_ids = gc_add(
		ft_calloc(sequence -> object_count, sizeof(pid_t)));
	int *const		pipes = gc_malloc(pipe_count * 2 * sizeof(int));

	object_index = 0;
	while (object_index < pipe_count)
		if (pipe(pipes + (object_index++) * 2) == -1)
			return (gc_free_ptr(pipes), 1);
	object_index = 0;
	while (object_index < sequence -> object_count)
	{
		process_ids[object_index] = fork();
		if (process_ids[object_index] == -1)
			return (/*free_pipeline_execution(process_ids, pipes),*/1);
		if (process_ids[object_index] == 0)
			ms_execute_pipeline_element(
				sequence, object_index, pipes, pipe_count);
		object_index++;
	}
	//main_signals();
	object_index = 0;
	while (object_index < pipe_count * 2)
		close(pipes[object_index++]);
	object_index = 0;
	while (object_index < sequence -> object_count - 1)
		waitpid(process_ids[object_index++], NULL, 0);
	puts("Reach");
	int exit_code = wait_for_process(process_ids[object_index]);
	return (exit_code);
}
