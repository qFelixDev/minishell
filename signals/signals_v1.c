/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_v1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:00:14 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/18 18:50:25 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>

void	sighandler(int sig)
{
	char	*prompt;

	(void)sig;
	prompt = create_prompt();
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	printf("\n%s", prompt);
	gc_free_ptr(prompt);
}

static void signal_exit(int sig)
{
	ms_exit(128 + sig);
}

void	main_signals(void)
{
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, signal_exit);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	command_signals(void)
{
	signal(SIGINT, signal_exit);
	signal(SIGQUIT, signal_exit);
}
