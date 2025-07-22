/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:02:00 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 00:03:49 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*static int	handle_plus_equals(char *arg, char *plus_equal_sign)
{
	char	key[MAX_VAR_LEN];
	char	value[MAX_VAR_LEN];
	char	*existing_value;
	char	*new_value;

	ft_strlcpy(key, arg, plus_equal_sign - arg);
	key[plus_equal_sign - arg] = '\0';
	if (key[0] == '\0')
		return (perror("minishell: export: `+=': not a valid identifier")
			, EXIT_FAILURE);
	ft_strlcpy(value, plus_equal_sign + 2, MAX_VAR_LEN);
	existing_value = ft_getenv(key);
	if (existing_value)
	{
		new_value = gc_add(ft_strjoin(existing_value, value));
		return (ms_set_env_value(key, new_value), gc_free_ptr(new_value), 1);
	}
	ms_set_env_value(key, value);
	return (0);
}

static void	handle_equal(char *arg, char *equal_sign, int *result)
{
	char	key[MAX_VAR_LEN];
	char	value[MAX_VAR_LEN];

	ft_strlcpy(key, arg, equal_sign - arg);
	key[equal_sign - arg] = '\0';
	if (key[0] == '\0')
	{
		perror("minishell: export: `=': not a valid identifier");
		return ;
	}
	ft_strlcpy(value, equal_sign + 1, MAX_VAR_LEN);
	ms_set_env_value(key, value);
	(void)result;
}

static void	find_signs(char **equal_sign, char **plus_equal_sign, char *arg)
{
	*equal_sign = ft_strchr(arg, '=');
	*plus_equal_sign = ft_strnstr(arg, "+=", ft_strlen(arg));
}

int	ms_export(t_ms_command *command)
{
	char	*equal_sign;
	char	*plus_equal_sign;
	int		i;
	int		result;

	result = 0;
	if (!command->argv[1])
		return (ms_print_env(), 0);
	i = 1;
	while (command->argv[i])
	{
		find_signs(&equal_sign, &plus_equal_sign, command->argv[i]);
		if (plus_equal_sign && plus_equal_sign != command->argv[i])
			result = handle_plus_equals(command->argv[i], plus_equal_sign);
		else if (equal_sign)
			handle_equal(command->argv[i], equal_sign, &result);
		else
		{
			ms_set_env_value(command->argv[i], NULL);
		}
		i++;
	}
	return (result);
}*/

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
			continue;
		}
		perform_operation(argv[argument_index], operator);
	}
	return (result);
}

