/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_v1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:00:14 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 15:03:10 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static void	sigtstp_command(int sig)
{
	(void)sig;
	destroy_minishell(131);
}

void	main_signals(void)
{
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, sigtstp_command);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
