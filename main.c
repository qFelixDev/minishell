/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:17:14 by reriebsc          #+#    #+#             */
/*   Updated: 2025/05/29 11:55:25 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	ms_exit(char **ms_envc)
{
	ms_free_envc(ms_envc);
	rl_clear_history();
	printf("exit\n");
}

int	main(int argc, char **argv, char **envp)
{
	int		exit_status;
	char	*input;

	char	*ms_envc;


	ms_envc = ms_generate_env(envp);
	set_man_environ(&ms_envc);

	return (ms_exit(ms_envc), exit_status);
}