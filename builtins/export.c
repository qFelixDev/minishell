/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:02:00 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 16:32:23 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_closest_operator(char *string)
{
	while (*string != '\0')
	{
		if (string[0] == '=')
			return (string);
		if (string[0] == '+' && string[1] == '=')
			return (string);
		string++;
	}
	return (string);
}

void	perform_operation(char *operation, char *string)
{
	char		*key;
	char		*value;

	if (string[0] == '\0')
		return ;
	key = gc_add(ft_substr(operation, 0, string - operation));
	if (string[0] == '=')
	{
		string += 1;
		ms_set_env_value(key, string);
	}
	else
	{
		string += 2;
		value = ft_getenv(key);
		if (value == NULL)
			ms_set_env_value(key, string);
		else
		{
			value = gc_add(ft_strjoin(value, string));
			ms_set_env_value(key, value);
			gc_free_ptr(value);
		}
	}
	gc_free_ptr(key);
}

bool	is_key_valid(char *key, char *terminator)
{
	if (key == terminator)
		return (false);
	while (key < terminator)
	{
		if (!ft_isalpha(*key) && *key != '_')
			return (false);
		key++;
	}
	return (true);
}

bool	is_argument_invalid(char *argument)
{
	if (*argument != '-')
		return (false);
	while (*argument == '-')
		argument++;
	return (*argument != '\0');
}

int	ms_export(char **argv)
{
	int		argument_index;
	int		result;
	char	*operator;

	if (argv[1] == NULL)
		return (ms_print_env(), 0);
	result = 0;
	argument_index = 0;
	while (argv[++argument_index] != NULL)
	{
		if (is_argument_invalid(argv[argument_index]))
			return (ft_putendl_fd("Invalid Usage", 2), 2);
		operator = get_closest_operator(argv[argument_index]);
		if (!is_key_valid(argv[argument_index], operator))
		{
			ft_putendl_fd("Invalid Key", 2);
			result = 1;
			continue ;
		}
		perform_operation(argv[argument_index], operator);
	}
	return (result);
}
