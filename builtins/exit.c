/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:02:16 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 13:56:55 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ms_exit_builtin(t_ms_command *command)
{
	if (command -> argv[1] == NULL)
		ms_exit(ms_minishell_get() -> exit_status);
	ms_exit(ft_atoi(command -> argv[1]));
	return (1);
}