/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:54:07 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/18 15:47:15 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

enum {
	MS_TOKEN_NONE = -1,
	MS_TOKEN_AND,
	MS_TOKEN_OR,
	MS_TOKEN_PIPE,
	MS_TOKEN_OPEN,
	MS_TOKEN_CLOSE,
	MS_TOKEN_DELIM,
	MS_TOKEN_APPEND,
	MS_TOKEN_INPUT,
	MS_TOKEN_OUTPUT,
	MS_TOKEN_HOME,
	MS_TOKEN_WILDCARD,
	MS_TOKEN_VARIABLE,
	MS_TOKEN_UNRESOLVED_STRING,
	MS_TOKEN_STRING,
	MS_TOKEN_SHADOW_STRING
};

typedef struct s_ms_token
{
	struct s_ms_token	*next;
	struct s_ms_token	*prev;
	int8_t				index;
	uint32_t			concatenation_serial;
}	t_ms_token;

char	*populate_token_content(
	t_ms_token *token, char *string, char terminator)
{
	size_t	length;

	length = 0;
	if (terminator == '\0' || terminator == '$')
		while (!ms_isspace(string[length])
			&& ft_strncmp(string + length, "&&", 2) != 0
			&& ft_strchr("|()<>~*$\"'", string[length]) == NULL)
			length++;
	else
		while (string[length] != terminator && string[length] != '\0')
			length++;
	if (terminator != '\0' && terminator != '$' && string[length] == '\0')
		return (NULL);
	token -> content = gc_malloc(length + 1);
	ft_strlcpy(token -> content, string, length + 1);
	return (string + ft_strlen(token -> content));
}

char	*populate_token(t_ms_token *token, char *string)
{
	const char *const	token_strings[MS_TOKEN_MAX] = {"&&", "||", "|", "(",
		")", "<<", ">>", "<", ">", "~", "*", "$", "\"", "'", ""};

	token -> index = MS_TOKEN_NONE;
	while (++token -> index < MS_TOKEN_SHADOW_STRING)
		if (ft_strncmp(string++, token_strings[token -> index],
				ft_strlen(token_strings[token -> index])) == 0)
			break ;
	string += ft_strlen(token_strings[token -> index]);
	if (token -> index < MS_TOKEN_VARIABLE)
		return (string);
	string = populate_token_content(token, string,
			*token_strings[token -> index]);
	if (string == NULL)
		return (NULL);
	if (token -> index != MS_TOKEN_VARIABLE)
		string += ft_strlen(token_strings[token -> index]);
	if (token -> index == MS_TOKEN_SHADOW_STRING)
		token -> index = MS_TOKEN_STRING;
	return (string);
}

t_ms_token	*ms_tokenize(char *string)
{
	t_ms_token *const	first = ms_insert_token(NULL, MS_TOKEN_OPEN);
	t_ms_token			*token;
	uint32_t			concatenation_serial;

	token = first;
	concatenation_serial = 1;
	while (true)
	{
		while (ms_isspace(*string))
			string++;
		string = populate_token(token, string);
		if (ms_isspace(*string))
			concatenation_serial++;
		token -> concatenation_serial = concatenation_serial
			* (token -> index >= MS_TOKEN_WILDCARD);
		if (string == NULL)
			return (ms_free_tokens(first), NULL);
		if (*string == '\0')
			return (token);
		token -> next = gc_add(ft_calloc(1, sizeof(t_ms_token)));
		token -> next -> prev = token;
		token = token -> next;
	}
}

