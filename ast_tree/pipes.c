/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:09:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/24 13:53:43 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>

int	exe_manager(t_ms_command *command, int delim_descriptor);

void	close_unused_pipes(int pipes[1024][2], size_t n_cmds, size_t i)
{
	size_t	j;

	j = 0;
	while (j < n_cmds - 1)
	{
		if (j != i -1)
			gc_close_fd(pipes[j][0]);
		if (j != i)
			gc_close_fd(pipes[j][1]);
		j++;
	}
}

int	init_pipes(t_ms_sequence *sequence, int pipes[1024][2])
{
	size_t	i;

	i = 0;
	while (i < sequence->object_count - 1)
	{
		pipe(pipes[i]);
		gc_add_fd(pipes[i][0]);
		gc_add_fd(pipes[i][1]);
		i++;
	}
	return (0);
}

void	child_process(t_ms_sequence *seq, int pipes[1024][2], size_t i)
{
	t_ms_command	*cmd;
	char			**env_cpy;

	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < seq->object_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_unused_pipes(pipes, seq->object_count, i);
	if (seq->is_sequence[i / 8] & (1u << (i % 8)))
		ms_exit(ms_execute_sequence(seq->objects[i]));
	else
	{
		cmd = ms_get_command(seq->objects[i]);
		cmd->delim_descriptor = seq->delim_descriptors[i];
		env_cpy = ms_gen_env();
		handle_child_process(cmd, env_cpy);
	}
}

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
