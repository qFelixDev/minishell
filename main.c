/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:17:14 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/05 19:41:59 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	ms_exit(int state)
{
	gc_list_clear(&ms_minishell_get()->env, free_env_entry);
	rl_clear_history();
	gc_close_fds();
	gc_free();
	//printf("exit\n");
	exit(state);
}

// 1) Building the Shell enviroment 
// 2) Modus switch for different Shell forms
// 3) Cleanup and exit the shell 
int	main(int ac, char **argv, char **envp)
{

	if (!ms_generate_env(envp))
		ms_exit(1);
	if (ac >= 2)
		non_interactive_arg(argv + 1, ac - 1);
	else if (isatty(STDIN_FILENO))
		interactive();
	else
		minishell_non_interactive();
	ms_exit(ms_minishell_get()->exit_status);
}
