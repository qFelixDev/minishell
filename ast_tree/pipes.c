/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:09:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/22 23:16:53 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>

int	exe_manager(t_ms_command *command);

//int	ft_process(t_ms_command *command)
//{
//	pid_t	pid;
//	int		p_fd[2];
//	int		res;
//
//	if (pipe(p_fd) == -1)
//		perror("pipe");
//	pid = fork();
//	if (pid == -1)
//		perror("fork");
//	if (pid == 0)
//	{
//		gc_close_fd(p_fd[0]);
//		dup2(p_fd[1], STDOUT_FILENO);
//		res = exe_manager(command);
//		ms_free_command(command);
//		ms_exit(res);
//	}
//	else
//	{
//		gc_close_fd(p_fd[1]);
//		dup2(p_fd[0], STDIN_FILENO);
//		res = wait_for_process(pid);
//		ms_free_command(command);
//	}
//	return (res);
//}
//
//void	pipe_process(
//	t_ms_sequence *sequence, size_t object_index, int *descriptors,
//	int std_descriptor)
//{
//	gc_close_fd(descriptors[0]);
//	dup2(descriptors[1], std_descriptor);
//	gc_close_fd(descriptors[1]);
//	if (sequence -> is_sequence[object_index / 8]
//		& (1u << (object_index % 8)))
//		ms_exit(ms_execute_sequence(sequence));
//	ms_exit(exe_manager(ms_get_command(sequence -> objects[object_index])));
//}
//
//void	pipe_fork_error(t_pipe_data *pipe_data)
//{
//	kill(pipe_data->left_pid, SIGKILL);
//	gc_close_fd(pipe_data->pipe_fds[0]);
//	gc_close_fd(pipe_data->pipe_fds[1]);
//	return (perror("Fork for right process failed"));
//}
//
//void	close_pipe_and_wait(t_pipe_data *pipe_data)
//{
//	gc_close_fd(pipe_data->pipe_fds[0]);
//	gc_close_fd(pipe_data->pipe_fds[1]);
//	waitpid(pipe_data->left_pid, NULL, 0);
//	waitpid(pipe_data->right_pid, &pipe_data->right_status, 0);
//	main_signals();
//}
//
//int	ms_execute_pipe(t_ms_sequence *sequence, size_t object_index)
//{
//	t_pipe_data	pipe_data;
//
//	reset_signals();
//	if (pipe(pipe_data.pipe_fds) == -1)
//		return (perror("Pipe creation failed"), 1);
//	pipe_data.left_pid = fork();
//	if (pipe_data.left_pid == -1)
//		return (perror("Fork for left process failed"), 1);
//	if (pipe_data.left_pid == 0)
//		pipe_process(sequence, object_index, (int[2]) {pipe_data.pipe_fds[0],
//			 pipe_data.pipe_fds[1]}, STDOUT_FILENO);
//	pipe_data.right_pid = fork();
//	if (pipe_data.right_pid == -1)
//		return (pipe_fork_error(&pipe_data), 1);
//	if (pipe_data.right_pid == 0)
//		pipe_process(sequence, object_index + 1,
//			(int[2]) {pipe_data.pipe_fds[1],pipe_data.pipe_fds[0]},
//				STDIN_FILENO);
//	close_pipe_and_wait(&pipe_data);
//	if (WIFEXITED(pipe_data.right_status))
//		pipe_data.right_result = WEXITSTATUS(pipe_data.right_status);
//	ms_minishell_get()->exit_status = pipe_data.right_result;
//	return (0);
//}
//
//void	ms_execute_pipeline_element(
//	t_ms_sequence *sequence, size_t object_index, int *pipes, size_t pipe_count)
//{
//	t_ms_command	*command;
//	size_t			pipe_index;
//
//	if (object_index != 0)
//		if (dup2(pipes[(object_index - 1) * 2], STDIN_FILENO) == -1)
//			ms_exit(1);	
//	if (object_index != pipe_count)
//		if (dup2(pipes[object_index * 2 + 1], STDOUT_FILENO) == -1)
//			ms_exit(1);
//	pipe_index = 0;
//	while (pipe_index < pipe_count * 2)
//		close(pipes[pipe_index++]);
//	if (sequence -> is_sequence[object_index / 8]
//		& (1u << (object_index % 8)))
//		ms_exit(ms_execute_sequence(sequence -> objects[object_index]));
//	command = ms_get_command(sequence -> objects[object_index]);
//	int exit_code = exe_manager(command);
//	ms_free_command(command);
//	ms_exit(exit_code);
//}
//
//int	ms_execute_pipeline(t_ms_sequence *sequence)
//{
//	size_t			object_index;
//	const size_t	pipe_count = sequence -> object_count - 1;
//	pid_t *const	process_ids = gc_add(
//		ft_calloc(sequence -> object_count, sizeof(pid_t)));
//	int *const		pipes = gc_malloc(pipe_count * 2 * sizeof(int));
//
//	object_index = 0;
//	while (object_index < pipe_count)
//		if (pipe(pipes + (object_index++) * 2) == -1)
//			return (gc_free_ptr(pipes), 1);
//	object_index = 0;
//	while (object_index < sequence -> object_count)
//	{
//		process_ids[object_index] = fork();
//		if (process_ids[object_index] == -1)
//			return (1);
//		if (process_ids[object_index] == 0)
//			ms_execute_pipeline_element(
//				sequence, object_index, pipes, pipe_count);
//		object_index++;
//	}
//	object_index = 0;
//	while (object_index < pipe_count * 2)
//		close(pipes[object_index++]);
//	object_index = 0;
//	while (object_index < sequence -> object_count - 1)
//		waitpid(process_ids[object_index++], NULL, 0);
//	puts("Reach");
//	int exit_code = wait_for_process(process_ids[object_index]);
//	return (exit_code);
//}
//int	pipe_monitor(t_ms_sequence *sequence)
//{
//	size_t	i;
//	int		pipes[1024][2];
//	pid_t	pid;
//	int		status;
//
//	for (i = 0; i < sequence->object_count - 1; i++)
//		if (pipe(pipes[i]) == -1)
//			return (perror("pipe"), 1);
//	for (i = 0; i < sequence->object_count; i++)
//	{
//		pid = fork();
//		if (pid == -1)
//			return (perror("fork"), 1);
//
//		if (pid == 0)
//		{
//			if (i > 0)
//				dup2(pipes[i - 1][0], STDIN_FILENO);
//			if (i < sequence->object_count - 1)
//				dup2(pipes[i][1], STDOUT_FILENO);
//			for (size_t j = 0; j < sequence->object_count - 1; j++)
//			{
//				close(pipes[j][0]);
//				close(pipes[j][1]);
//			}
//			if (sequence->is_sequence[i / 8] & (1u << (i % 8)))
//				exit(ms_execute_sequence(sequence->objects[i]));
//			else
//			{
//				t_ms_command *cmd = ms_get_command(sequence->objects[i]);
//				int result = exe_manager(cmd);
//				ms_free_command(cmd);
//				exit(result);
//			}
//		}
//	}
//	for (i = 0; i < sequence->object_count - 1; i++)
//	{
//		close(pipes[i][0]);
//		close(pipes[i][1]);
//	}
//	for (i = 0; i < sequence->object_count; i++)
//		wait(&status);
//	return (WEXITSTATUS(status));
//}

static void	close_unused_pipes(int pipes[1024][2], size_t n_cmds, size_t i)
{
	for (size_t j = 0; j < n_cmds - 1; j++)
	{
		if (j != i - 1)
			close(pipes[j][0]);
		if (j != i)
			close(pipes[j][1]);
	}
}

void	close_all_pipes(t_ms_sequence *seq, int pipes[1024][2])
{
	size_t	i;

	i = 0;
	while (i < seq->object_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	init_pipes(t_ms_sequence *sequence, int pipes[1024][2])
{
	size_t	i;

	i = 0;
	while (i < sequence->object_count - 1)
	{
		if (pipe(pipes[i]) == -1)
			return (perror("pipe"), 1);
		i++;
	}
	return (0);
}

//void	child_process(t_ms_sequence *seq, int pipes[1024][2], size_t i)
//{
//	size_t			j;
//	int				result;
//	t_ms_command	*cmd;
//
//	j = 0;
////	if (i > 0)
////		dup2(pipes[i - 1][0], STDIN_FILENO);
////	if (i < seq->object_count - 1)
////		dup2(pipes[i][1], STDOUT_FILENO);
//	if (i > 0)
//	{
//		dup2(pipes[i - 1][0], STDIN_FILENO);
//		close(pipes[i - 1][0]); // WICHTIG!
//	}
//	if (i < seq->object_count - 1)
//	{
//		dup2(pipes[i][1], STDOUT_FILENO);
//		close(pipes[i][1]); // WICHTIG!
//	}
//
//	while (j < seq->object_count - 1)
//	{
//		close(pipes[j][0]);
//		close(pipes[j][1]);
//		j++;
//	}
//	if (seq->is_sequence[i / 8] & (1u << (i % 8)))
//		exit(ms_execute_sequence(seq->objects[i]));
//	else
//	{
//		cmd = ms_get_command(seq->objects[i]);
//		result = exe_manager(cmd);
//		ms_free_command(cmd);
//		exit(result);
//	}
//}

void	child_process(t_ms_sequence *seq, int pipes[1024][2], size_t i)
{
	t_ms_command	*cmd;

	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < seq->object_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);

	close_unused_pipes(pipes, seq->object_count, i);

	if (seq->is_sequence[i / 8] & (1u << (i % 8)))
		exit(ms_execute_sequence(seq->objects[i]));
	else
	{
		cmd = ms_get_command(seq->objects[i]);
		cmd->delim_descriptor = seq->delim_descriptors[i];
		char **env_cpy = ms_gen_env();
		handle_child_process(cmd, env_cpy);
	}
}


//void	child_process(t_ms_sequence *seq, int pipes[1024][2], size_t i)
//{
//	t_ms_command	*cmd;
//	int				result;
//
//	if (i > 0)
//	{
//		dup2(pipes[i - 1][0], STDIN_FILENO);
//	}
//	if (i < seq->object_count - 1)
//	{
//		dup2(pipes[i][1], STDOUT_FILENO);
//	}
//
//	close_unused_pipes(pipes, seq->object_count, i);
//
//	if (seq->is_sequence[i / 8] & (1u << (i % 8)))
//		exit(ms_execute_sequence(seq->objects[i]));
//	else
//	{
//		cmd = ms_get_command(seq->objects[i]);
//		result = exe_manager(cmd);
//		ms_free_command(cmd);
//		exit(result);
//	}
//}

int	fork_children(t_ms_sequence *sequence, int pipes[1024][2])
{
	size_t	i;
	pid_t	pid;

	i = 0;
	while (i < sequence->object_count)
	{
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
			child_process(sequence, pipes, i);
		i++;
	}
	return (0);
}

void	wait_for_children(t_ms_sequence *seq, int *status)
{
	size_t	i;

	i = 0;
	while (i < seq->object_count)
	{
		wait(status);
		i++;
	}
}
