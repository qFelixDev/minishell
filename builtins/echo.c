/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:46:13 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/08 10:14:35 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (false);
	i = 2;
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

static int	process_echo_options(t_ms_command *cmd, bool *is_n)
{
	int	i;

	i = 1;
	while (cmd->argv[i] && is_n_flag(cmd->argv[i]))
	{
		*is_n = true;
		i++;
	}
	return (i);
}

static void	print_echo_arguments(t_ms_command *cmd, int start_idx)
{
	bool	first_word;
	int		i;

	first_word = true;
	i = start_idx;
	while (cmd->argv[i])
	{
		if (!first_word)
			ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
		first_word = false;
		i++;
	}
}

int	ms_echo(t_ms_command *command, bool is_n)
{
	int	start_idx;

	if (!command->argv || !command->argv[1])
	{
		if (!is_n)
			ft_putchar_fd('\n', STDOUT_FILENO);
		return (0);
	}
	start_idx = process_echo_options(command, &is_n);
	print_echo_arguments(command, start_idx);
	if (!is_n)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
