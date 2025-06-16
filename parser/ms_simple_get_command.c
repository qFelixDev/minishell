/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_simple_get_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 10:52:29 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/16 09:27:06 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define MS_TOKENS
#include <stdio.h>
#include "../includes/minishell.h"
//#include "token.h"
#include "../libft_extend/libft.h"

static int	count_arguments(t_ms_token *token)
{
	int	count;

	count = 0;
	while (token != NULL)
	{
		if (token -> index < MS_TOKEN_UNRESOLVED_STRING)
			return (printf("ms_get_command only works with resolved, "
				"non concatenated strings as of now\n"), -1);
		token = token -> next;
		count++;
	}
	return (count);
}

static bool	expand_variables(t_ms_token *token)
{
	char	*value;

	while (token != NULL)
	{
		if (token -> index == MS_TOKEN_VARIABLE)
		{
			value = ft_strdup(getenv(token -> content));
			if (value == NULL)
				return (false);
			free(token -> content);
			token -> index = MS_TOKEN_STRING;
			token -> content = value;
		}
		token = token -> next;
	}
	return (true);
}

t_ms_command	*ms_get_command(t_ms_token *token)
{
	const int		argument_count = count_arguments(token);
	int				argument_index;
	t_ms_command	*command;

	if (argument_count == -1)
		return (NULL);
	if (!expand_variables(token))
		return (NULL);
	command = ft_calloc(1, sizeof(t_ms_command));
	if (command == NULL)
		return (NULL);
	command -> argv = ft_calloc(argument_count + 1, sizeof(char *));
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

void	ms_free_command(t_ms_command *command)
{
	char	**argv;

	argv = command -> argv;
	while (*argv != NULL)
		free(*(argv++));
	free(command -> argv);
	free(command);
}

void	ms_print_command(t_ms_command *command)
{
	char	**argv;

	argv = command -> argv;
	while (*argv != NULL)
		printf("%s ", *(argv++));
	printf("\n");
}
