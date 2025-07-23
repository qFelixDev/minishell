/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:02:00 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 11:17:54 by reriebsc         ###   ########.fr       */
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
		operator = get_closest_operator(argv[argument_index]);
		if (!is_key_valid(argv[argument_index], operator))
		{
			fprintf(stderr, "Invalid Key\n");
			result = 1;
			continue ;
		}
		perform_operation(argv[argument_index], operator);
	}
	return (result);
}

