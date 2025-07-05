/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:02:00 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/05 15:35:05 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//static void	print_invalid_identifier_error(const char *key)
//{
//	write(STDERR_FILENO, RED, ft_strlen(RED));
//	write(STDERR_FILENO, "minishell: export: `", 20);
//	write(STDERR_FILENO, key, ft_strlen(key));
//	write(STDERR_FILENO, "': not a valid identifier", 26);
//	write(STDERR_FILENO, RESET, ft_strlen(RESET));
//	write(STDERR_FILENO, "\n", 1);
//}

static int	handle_plus_equals(char *arg, char *plus_equal_sign)
{
	char	key[MAX_VAR_LEN];
	char	value[MAX_VAR_LEN];
	char	*existing_value;
	char	*new_value;

	ft_strlcpy(key, arg, plus_equal_sign - arg);
	key[plus_equal_sign - arg] = '\0';
	if (key[0] == '\0')
		return (perror("minishell: export: `+=': not a valid identifier"), EXIT_FAILURE);
	//if (!is_valid_identifier(key))
	//	return (print_invalid_identifier_error(key), 1);
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
	//if (!is_valid_identifier(key))
	//{
	//	print_invalid_identifier_error(key);
	//	*result = 1;
	//	return ;
	//}
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
			//if (!is_valid_identifier(command->argv[i]))
			//	return (perror(ERROR_EXPORT), EXIT_FAILURE);
			ms_set_env_value(command->argv[i], NULL);
		}
		i++;
	}
	return (result);
}
