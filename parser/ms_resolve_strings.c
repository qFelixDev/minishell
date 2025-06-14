/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_resolve_strings.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 12:24:58 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/11 09:21:26 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_extend/libft.h"
#include "token.h"

t_ms_token	*add_string_token(t_ms_token *token, char *string)
{
	int8_t	index;
	size_t	length;

	index = MS_TOKEN_STRING;
	if (*string == '$')
	{
		index = MS_TOKEN_VARIABLE;
		string++;
	}
	length = 0;
	while (string[length] != '$' && string[length] != '\0'
		&& (index != MS_TOKEN_VARIABLE || !ms_isspace(string[length])))
		length++;
	token = ms_insert_token(token, index);
	if (token == NULL)
		return (NULL);
	token -> content = malloc(length + 1);
	if (token -> content == NULL)
		return (NULL);
	ft_strlcpy(token -> content, string, length + 1);
	return (token);
}

bool	ms_resolve_strings(t_ms_token *token)
{
	char	*string;
	int8_t	index;
	size_t	length;

	while (token != NULL)
	{
		if (token -> index == MS_TOKEN_UNRESOLVED_STRING)
		{
			string = token -> content;
			while (*string != '\0')
			{
				token = add_string_token(token, string);
				if (token == NULL)
					return (false);
				string += (token -> index == MS_TOKEN_VARIABLE)
					&& ft_strlen(token -> content);
			}
		}
		token = token -> next;
	}
	return (true);
}
