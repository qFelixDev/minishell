/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:02:16 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/23 16:59:10 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_numeric(char *string)
{
	const char *const	max_string = "9223372036854775807";
	size_t				length;
	size_t				index;
	const bool			is_negative = (*string == '-');

	while (ms_isspace(*string))
		string++;
	string += (*string == '+' || *string == '-');
	length = 0;
	while (ft_isdigit(string[length]))
		length++;
	index = 0;
	while (string[length + index] != '\0')
		if (!ms_isspace(string[length + index]))
			return (false);
	if (length != 19)
		return (length < 19 && length != 0);
	index = 0;
	while (index < 18)
	{
		if (string[index] != max_string[index])
			return (string[index] < max_string[index]);
		index++;
	}
	return (string[index] <= max_string[index] + (is_negative));
}

int64_t	atoi64(char *string)
{
	const bool	is_negative = (*string == '-');
	int64_t		number;

	while (ms_isspace(*string))
		string++;
	string += (*string == '+' || *string == '-');
	number = 0;
	while (ft_isdigit(*string))
		number = number * 10 + (1 - 2 * is_negative) * (*(string++) - '0');
	return (number);
}

int	ms_exit_builtin(t_ms_command *command)
{
	int	argc;

	if (command -> argv[1] != NULL && !is_numeric(command -> argv[1]))
	{
		ft_putendl_fd("Numeric argument required", 2);
		ms_exit(255);
	}
	argc = 0;
	while (command -> argv[argc] != NULL)
		argc++;
	if (argc > 2)
		return (ft_putendl_fd("Too many arguments", 2), 1);
	if (command -> argv[1] == NULL)
		ms_exit(ms_minishell_get()->exit_status);
	ms_exit(atoi64(command -> argv[1]));
	return (1);
}
