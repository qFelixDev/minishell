/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:38:14 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 19:06:03 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>

void	close_all_pipes(t_ms_sequence *seq, int pipes[1024][2])
{
	size_t	i;

	i = 0;
	while (i < seq->object_count - 1)
	{
		gc_close_fd(pipes[i][0]);
		gc_close_fd(pipes[i][1]);
		i++;
	}
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

int	pipe_monitor(t_ms_sequence *sequence)
{
	int		pipes[1024][2];
	int		status;

	if (init_pipes(sequence, pipes) != 0)
		return (1);
	if (fork_children(sequence, pipes) != 0)
		return (1);
	close_all_pipes(sequence, pipes);
	wait_for_children(sequence, &status);
	ms_minishell_get()->exit_status = WEXITSTATUS(status);
	return (WEXITSTATUS(status));
}
