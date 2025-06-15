/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:13:11 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/15 12:53:31 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_extend/libft.h"
#include "token.h"

char	*populate_token_content(
	t_ms_token *token, char *string, char terminator)
{
	size_t	length;

	length = 0;
	if (terminator == '\0' || terminator == '$')
		while (!ms_isspace(string[length])
			&& ft_strncmp(string + length, "&&", 2) != 0
			&& ft_strchr("|()<>*$\"'", string[length]) == NULL)
			length++;
	else
		while (string[length] != terminator && string[length] != '\0')
			length++;
	if (string[length] != '\0')
		token -> content = malloc(length + 1);
	if (token -> content == NULL)
		return (NULL);
	ft_strlcpy(token -> content, string, length + 1);
	string += ft_strlen(token -> content);
	return (string);
}

char	*populate_token(t_ms_token *token, char *string)
{
	const char *const	token_strings[MS_TOKEN_MAX] = {"&&", "||", "|",
		"(", ")", "<<", ">>", "<", ">", "*", "$", "\"", "'", ""};

	token -> next = NULL;
	token -> index = MS_TOKEN_NONE;
	while (++token -> index < MS_TOKEN_SHADOW_STRING)
		if (ft_strncmp(string, token_strings[token -> index],
				ft_strlen(token_strings[token -> index])) == 0)
			break ;
	string += ft_strlen(token_strings[token -> index]);
	if (token -> index < MS_TOKEN_VARIABLE)
		return (string);
	token -> content = NULL;
	string = populate_token_content(token, string,
			*token_strings[token->index]);
	if (string == NULL)
		return (NULL);
	if (token -> index != MS_TOKEN_VARIABLE)
		string += ft_strlen(token_strings[token -> index]);
	token -> concatenate_content = ms_isspace(*string);
	if (token -> index == MS_TOKEN_SHADOW_STRING)
		token -> index = MS_TOKEN_STRING;
	return (string);
}

t_ms_token	*ms_tokenize(char *string)
{
	t_ms_token *const	first = ms_insert_token(NULL, MS_TOKEN_OPEN);
	t_ms_token			*token;

	if (first == NULL)
		return (NULL);
	token = first;
	while (ms_isspace(*string))
		string++;
	while (*string != '\0')
	{
		token -> next = malloc(sizeof(t_ms_token));
		token = token -> next;
		if (token == NULL)
			return (ms_free_tokens(first, false), NULL);
		string = populate_token(token, string);
		if (string == NULL)
			return (ms_free_tokens(first, false), NULL);
		while (ms_isspace(*string))
			string++;
	}
	if (ms_insert_token(token, MS_TOKEN_CLOSE) == NULL)
		return (ms_free_tokens(first, false), NULL);
	return (first);
}
