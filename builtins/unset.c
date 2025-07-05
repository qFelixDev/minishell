/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:02:20 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/05 15:26:19 by reriebsc         ###   ########.fr       */
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

int	ms_unset(t_ms_command *command)
{
	size_t	i;
	int		result;

	i = 0;
	result = 0;
	while (command->argv[i])
	{
		//if (!is_valid_identifier(command->argv[i]))
		//{
		//	write(STDERR_FILENO, RED, ft_strlen(RED));
		//	write(STDERR_FILENO, "minishell: unset: `", 20);
		//	write(STDERR_FILENO, command->argv[i], ft_strlen(command->argv[i]));
		//	write(STDERR_FILENO, "': not a valid identifier", 26);
		//	write(STDERR_FILENO, RESET, ft_strlen(RESET));
		//	write(STDERR_FILENO, "\n", 1);
		//	result = 1;
		//}
		remove_variable_from_env(command->argv[i]);
		i++;
	}
	return (result);
}