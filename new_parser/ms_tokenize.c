/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 15:54:07 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/19 17:17:30 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#include<string.h>
#include<stdio.h>

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
	MS_TOKEN_SHADOW_STRING,
	MS_TOKEN_MAX
};

typedef struct s_ms_token
{
	struct s_ms_token	*next;
	struct s_ms_token	*prev;
	int8_t				index;
	char				*content;
	uint32_t			concatenation_serial;
}	t_ms_token;

void	ms_free_tokens(t_ms_token *token)
{
	t_ms_token	*temp;

	while (token != NULL)
	{
		temp = token;
		token = token -> next;
		if (temp -> index >= MS_TOKEN_WILDCARD)
			free(temp -> content);
		free(temp);
	}
}

char	*populate_token_content(
	t_ms_token *token, char *string, char terminator)
{
	size_t	length;

	length = 0;
	if (terminator == '\0' || terminator == '$')
		while (!isspace(string[length])
			&& strncmp(string + length, "&&", 2) != 0
			&& strchr("|()<>~*$\"'", string[length]) == NULL)
			length++;
	else
		while (string[length] != terminator && string[length] != '\0')
			length++;
	if (terminator != '\0' && terminator != '$' && string[length] == '\0')
		return (NULL);
	token -> content = malloc(length + 1);
	strlcpy(token -> content, string, length + 1);
	return (string + strlen(token -> content));
}

char	*populate_token(t_ms_token *token, char *string)
{
	const char *const	token_strings[MS_TOKEN_MAX] = {"&&", "||", "|", "(",
		")", "<<", ">>", "<", ">", "~", "*", "$", "\"", "'", ""};

	token -> index = MS_TOKEN_NONE;
	while (++token -> index < MS_TOKEN_SHADOW_STRING)
		if (strncmp(string, token_strings[token -> index],
				strlen(token_strings[token -> index])) == 0)
			break ;
	string += strlen(token_strings[token -> index]);
	if (token -> index < MS_TOKEN_VARIABLE)
		return (string);
	string = populate_token_content(token, string,
			*token_strings[token -> index]);
	if (string == NULL)
		return (NULL);
	if (token -> index != MS_TOKEN_VARIABLE)
		string += strlen(token_strings[token -> index]);
	if (token -> index == MS_TOKEN_SHADOW_STRING)
		token -> index = MS_TOKEN_STRING;
	return (string);
}

t_ms_token	*ms_insert_token(t_ms_token *token, int8_t index)
{
	t_ms_token *const	new_token = calloc(1, sizeof(t_ms_token));

	new_token -> index = index;
	if (token == NULL)
		return (new_token);
	new_token -> next = token -> next;
	if (new_token -> next != NULL)
		new_token -> next -> prev = new_token;
	token -> next = new_token;
	new_token -> prev = token;
	return (new_token);
}

t_ms_token	*ms_tokenize(char *string)
{
	t_ms_token *const	first = ms_insert_token(NULL, MS_TOKEN_OPEN);
	t_ms_token			*token;
	uint32_t			concatenation_serial;

	token = first;
	concatenation_serial = 1;
	while (*string != '\0')
	{
		token -> next = calloc(1, sizeof(t_ms_token));
		token -> next -> prev = token;
		token = token -> next;
		string = populate_token(token, string);
		if (string == NULL)
			return (ms_free_tokens(first), NULL);
		token -> concatenation_serial = concatenation_serial
			* (token -> index >= MS_TOKEN_HOME);
		if (isspace(*string))
			token -> concatenation_serial++;
		while (isspace(*string))
			string++;
	}
	ms_insert_token(token, MS_TOKEN_CLOSE);
	return (first);
}

char	*ms_get_token_identity(int8_t index)
{
	static char	*identities[MS_TOKEN_MAX] = {"&&", "||", "|", "(", ")", "<<",
		">>", "<", ">", "~", "*", "$STR", "\"STR\"", "'STR'", "STR"};
	return (identities[index]);
}

void	ms_print_tokens(t_ms_token *token)
{
	while (token != NULL)
	{
		printf("%s ", ms_get_token_identity(token -> index));
		token = token -> next;
	}
	printf("\n");
}

int main() {
	t_ms_token* token = ms_tokenize("ls | ls \"heifijefij\"");
	ms_print_tokens(token);
	ms_free_tokens(token);
}