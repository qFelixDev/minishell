/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:02:20 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 17:01:00 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	remove_variable_from_env(const char *key)
{
	const size_t	key_len = ft_strlen(key);
	t_list			*current;
	t_list			*previous;
	t_dict_env		*entry;

	previous = NULL;
	current = ms_minishell_get()->env;
	while (current)
	{
		entry = current->content;
		if (ft_strncmp(entry->key, key, key_len) == 0)
		{
			if (current == ms_minishell_get()->env)
				ms_minishell_get()->env = current->next;
			else
				previous->next = current->next;
			free_env_entry(entry);
			gc_free_ptr(current);
			return (true);
		}
		previous = current;
		current = current->next;
	}
	return (false);
}

static bool	is_argument_invalid(char *argument)
{
	if (*argument != '-')
		return (false);
	while (*argument == '-')
		argument++;
	return (*argument != '\0');
}

int	ms_unset(t_ms_command *command)
{
	size_t	i;
	int		result;

	i = 0;
	result = 0;
	while (command->argv[i])
	{
		if (is_argument_invalid(command -> argv[i]))
			return (ft_putendl_fd("Invalid Usage", 2), 2);
		if (!is_valid_identifier(command -> argv[i]))
		{
			ft_putendl_fd("Invalid identifier", 2);
			result = 1;
			i++;
			continue ;
		}
		remove_variable_from_env(command->argv[i]);
		i++;
	}
	return (result);
}
