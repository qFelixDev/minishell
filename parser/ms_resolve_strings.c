/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_resolve_strings.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 12:24:58 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/13 18:06:26 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_extend/libft.h"
#include "token.h"

t_ms_token	*add_string_token(t_ms_token *token, char *string)
{
	int8_t	index;
	size_t	length;

	index = MS_TOKEN_STRING * (*string != '$')
		+ MS_TOKEN_VARIABLE * (*string == '$');
	string += (*string == '$');
	length = 0;
	if (*string == '$' || *string == '?')
		length = 1;
	else
		while (string[length] != '$' && string[length] != '\0'
			&& (index != MS_TOKEN_VARIABLE || ft_isalnum(string[length])
				|| string[length] == '_'))
			length++;
	if (token -> index != MS_TOKEN_UNRESOLVED_STRING)
		token = ms_insert_token(token, index);
	else
		token -> index = index;
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
	int		string_index;

	while (token != NULL)
	{
		if (token -> index == MS_TOKEN_UNRESOLVED_STRING)
		{
			string = token -> content;
			string_index = 0;
			while (string[string_index] != '\0')
			{
				if (token -> index != MS_TOKEN_UNRESOLVED_STRING)
					token -> concatenate_content = true;
				token = add_string_token(token, string + string_index);
				if (token == NULL)
					return (free(string), false);
				string_index += (token -> index == MS_TOKEN_VARIABLE)
					+ ft_strlen(token -> content);
			}
			//free(string);
		}
		token = token -> next;
	}
	return (true);
}
