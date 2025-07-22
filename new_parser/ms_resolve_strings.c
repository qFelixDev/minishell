/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_resolve_strings.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:55:12 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 11:45:17 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_ms_token	*insert_string_token(t_ms_token *token, char *string)
{
	int8_t	index;
	size_t	length;

	index = MS_TOKEN_STRING;
	if (string[0] == '$' && (ft_isalnum(string[1]) || string[1] == '_'
			|| string[1] == '$' || string[1] == '?'))
		index == MS_TOKEN_VARIABLE;
	string += (index == MS_TOKEN_VARIABLE);
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
	token -> content = gc_malloc(length + 1);
	return (ft_strlcpy(token -> content, string, length + 1), token);
}

void	ms_resolve_strings(t_ms_token *token)
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
				token = insert_string_token(token, string + string_index);
				string_index += (token -> index == MS_TOKEN_VARIABLE)
					+ ft_strlen(token -> content);
			}
		}
		token = token -> next;
	}
	return (true);
}
