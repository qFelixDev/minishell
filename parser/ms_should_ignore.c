/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_should_ignore.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 18:03:41 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/23 18:24:40 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	ms_should_ignore(t_ms_token *token)
{
	char	*value;

	token = token -> next;
	while (token -> index != MS_TOKEN_CLOSE)
	{
		if (token -> index != MS_TOKEN_VARIABLE)
			return (false);
		value = ft_getenv(token -> content);
		if (value != NULL)
		{
			while (ms_isspace(*value))
				value++;
			if (*value != '\0')
				return (false);
		}
		token = token -> next;
	}
	return (true);
}