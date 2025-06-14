/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 10:52:29 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/14 17:34:36 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

bool	ms_expand_variables(t_ms_token *token)
{
	while (token != NULL)
	{
		if (token -> index == MS_TOKEN_VARIABLE)
		{
			value = ft_strdup(getenv(token -> content));
			if (value == NULL)
				return (NULL);
			free(token -> content);
			token -> index = MS_TOKEN_STRING;
			token -> content = value;
		}
		token = token -> next;
	}
}

t_ms_command	*allocate_command(t_ms_token *token)
{
	argument_count = 0;
	while (token != NULL)
	{
		if (token -> index >= MS_UNRESOLVED_STRING)
			return (printf("ms_get_command only works with resolved, "
				"non concatenated strings as of now"), NULL);
		token = token -> next;
		argument_count++;
	}
}

int	count_arguments(t_ms_token *token)
{
	int	count;

	count = 0;
	while (token != NULL)
	{
		if (token -> index >= MS_UNRESOLVED_STRING)
			return (printf("ms_get_command only works with resolved, "
				"non concatenated strings as of now"), -1);
		token = token -> next;
		count++;
	}
	return (count);
}

t_ms_command	*ms_get_command(t_ms_token *token)
{
	const int		argument_count = count_arguments(token);
	int				argument_index;
	t_ms_command	*command;

	if (argument_count == -1)
		return (NULL);
	command = ft_calloc(1, sizeof(t_ms_command));
	if (command == NULL)
		return (NULL);
	command -> argv = ft_calloc(argument_count + 1, sizeof(char*));
	if (command -> argv == NULL)
		return (free(command), NULL);
	argument_index = 0;
	while (true)
	{
		command -> argv[argument_index] = ft_strdup(token -> content);
		if (command -> argv[argument_index++] == NULL)
			break ;
		if (argument_index == argument_count)
			return (command);
		token = token -> next;
	}
	while (argument_index-- > 0)
		free(command -> argv[argument_index]);
	return (free(command), NULL);
}
