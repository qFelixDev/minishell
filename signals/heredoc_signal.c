/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:50:56 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 14:51:12 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	heredoc_sighandler(int sig)
{
	(void)sig;
	unlink("heredoc_temp.txt");
	destroy_minishell(130);
}

void	signal_heredoc(void)
{
	signal(SIGINT, heredoc_sighandler);
	signal(SIGTSTP, SIG_IGN);
}
