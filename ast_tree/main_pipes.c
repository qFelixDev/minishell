/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:38:14 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/19 17:44:19 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>
#include <sys/wait.h>

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
	return (WEXITSTATUS(status));
}
