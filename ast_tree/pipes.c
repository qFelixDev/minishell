/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:09:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/19 13:17:07 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>

int	exe_manager(t_ms_command *command);

/*void	pipe_left_process(t_ms_sequence *sequence, t_pipe_data *pipe_data)
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
}*/

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
		ms_exit(res);
	}
	else
	{
		gc_close_fd(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		res = wait_for_process(pid);
		ms_free_command(command);
	}
	return (res);
}

//int	pipe_monitor(t_ms_sequence *sequence)
//{
//	size_t			pipe_index;
//	int				exit_code;
//
//	pipe_index = 0;
//	while (pipe_index < sequence -> object_count - 1)
//	{
//		//if (sequence -> is_sequence[pipe_index / 8]
//		//	& (1u << (pipe_index % 8)))
//		if (!(sequence -> is_sequence[pipe_index / 8]
//			& (1u << (pipe_index % 8))))
//			exit_code = ft_process(ms_get_command(sequence->objects[pipe_index]));
//		pipe_index++;
//	}
//	return (exit_code);
//}

/*int	pipe_monitor(t_ms_sequence *sequence)
{
	size_t	i;
	int		pipes[1024][2]; // genug Platz für viele Pipes
	pid_t	pid;
	int		status;

	// Öffne alle benötigten Pipes vorab
	for (i = 0; i < sequence->object_count - 1; i++)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return (1);
		}
	}

	// Fork für jedes Kommando
	for (i = 0; i < sequence->object_count; i++)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid == 0)
		{
			// Falls nicht erstes Kommando → STDIN von vorheriger Pipe
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			// Falls nicht letztes Kommando → STDOUT in nächste Pipe
			if (i < sequence->object_count - 1)
				dup2(pipes[i][1], STDOUT_FILENO);

			// Schließe alle Pipes im Child
			for (size_t j = 0; j < sequence->object_count - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			int result = exe_manager(ms_get_command(sequence->objects[i]));
			ms_free_command(ms_get_command(sequence->objects[i]));
			ms_exit(result); // sauber beenden
		}
	}

	// Parent: schließt alle Pipes
	for (i = 0; i < sequence->object_count - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	// Warte auf alle Kindprozesse
	for (i = 0; i < sequence->object_count; i++)
		wait(&status);

	return (WEXITSTATUS(status));
}*/



/*static void	ft_here_doc_input(char *limiter, int write_fd)
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
}*/

#include "../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>

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

/*int	pipe_monitor(t_ms_sequence *sequence)
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
}*/

void	ms_execute_pipeline_element(
	t_ms_sequence *sequence, size_t object_index, int *pipes, size_t pipe_count)
{
	t_ms_command	*command;
	size_t			pipe_index;

	//reset_signals();
	if (object_index != 0)
		if (dup2(pipes[(object_index - 1) * 2], STDIN_FILENO) == -1)
			ms_exit(1);	
	if (object_index != pipe_count)
		if (dup2(pipes[object_index * 2 + 1], STDOUT_FILENO) == -1)
			ms_exit(1);
	pipe_index = 0;
	while (pipe_index < pipe_count * 2)
		close(pipes[pipe_index++]);
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


int	pipe_monitor(t_ms_sequence *sequence)
{
	size_t	i;
	int		pipes[1024][2];
	pid_t	pid;
	int		status;

	for (i = 0; i < sequence->object_count - 1; i++)
		if (pipe(pipes[i]) == -1)
			return (perror("pipe"), 1);

	for (i = 0; i < sequence->object_count; i++)
	{
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);

		if (pid == 0)
		{
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			if (i < sequence->object_count - 1)
				dup2(pipes[i][1], STDOUT_FILENO);

			for (size_t j = 0; j < sequence->object_count - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			// 👉 Jetzt prüfen: Subsequence oder Command?
			if (sequence->is_sequence[i / 8] & (1u << (i % 8)))
				exit(ms_execute_sequence(sequence->objects[i]));
			else
			{
				t_ms_command *cmd = ms_get_command(sequence->objects[i]);
				int result = exe_manager(cmd);
				ms_free_command(cmd);
				exit(result);
			}
		}
	}

	for (i = 0; i < sequence->object_count - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	for (i = 0; i < sequence->object_count; i++)
		wait(&status);

	return (WEXITSTATUS(status));
}
