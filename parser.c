/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:31:52 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/04 15:55:49 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft_extend/libft.h"

enum
{
	MS_TOKEN_SPACE = -1,
	MS_TOKEN_AND,
	MS_TOKEN_OR,
	MS_TOKEN_PIPE,
	MS_TOKEN_DELIM,
	MS_TOKEN_APPEND,
	MS_TOKEN_INPUT,
	MS_TOKEN_OUTPUT,
	MS_TOKEN_OPEN,
	MS_TOKEN_CLOSE,
	MS_TOKEN_STRING_OPAQUE,
	MS_TOKEN_STRING_SEMI,
	MS_TOKEN_STRING_TRANSPARENT,
	MS_TOKEN_MAX
};

typedef struct s_ms_token {
	struct s_ms_token	*next;
	int					index;
	char				*string;
	bool				concatenated;
}	t_ms_token;

void	ms_free_tokens(t_ms_token *token)
{
	t_ms_token	*temp;

	while (token != NULL)
	{
		temp = token;
		token = token -> next;
		if (temp -> index >= MS_TOKEN_STRING_OPAQUE)
			free(temp -> string);
		free(temp);
	}
}

bool	ms_isspace(char character)
{
	return ((character >= 9 && character <= 13) || character == 32);
}

char	*populate_token_content(
	t_ms_token *token, char *string, char terminator)
{
	size_t	length;

	length = 0;
	if (token -> index == MS_TOKEN_STRING_TRANSPARENT)
	{
		while (!ms_isspace(string[length])
			&& ft_strncmp(string + length, "&&", 2) != 0
			&& ft_strchr("|<>()'\"", string[length]) == NULL)
			length++;
	}
	else
	{
		while (string[length] != terminator && string[length] != '\0')
			length++;
		if (string[length] == '\0')
			return (NULL);
	}
	token -> string = malloc(length + 1);
	if (token -> string == NULL)
		return (NULL);
	ft_strlcpy(token -> string, string, length + 1);
	string += length + (token -> index != MS_TOKEN_STRING_TRANSPARENT);
	return (string);
}

char	*populate_token(t_ms_token *token, char *string)
{
	char const *const	token_strings[12]
		= {"&&", "||", "|", "<<", ">>", "<", ">", "(", ")", "'", "\"", ""};

	token -> index = MS_TOKEN_SPACE;
	if (ms_isspace(*string))
	{
		while (ms_isspace(*string))
			string++;
		return (string);
	}
	while (++token -> index < MS_TOKEN_STRING_TRANSPARENT)
		if (ft_strncmp(string, token_strings[token -> index],
				ft_strlen(token_strings[token -> index])) == 0)
			break ;
	string += ft_strlen(token_strings[token -> index]);
	if (token -> index < MS_TOKEN_STRING_OPAQUE)
		return (string);
	string = populate_token_content(
		token, string, *token_strings[token -> index]);
	if (string == NULL)
		token -> index = MS_TOKEN_SPACE;
	return (string);
}

#include<unistd.h>

t_ms_token	*ms_tokenize(char *string)
{
	t_ms_token *const	first = malloc(sizeof(t_ms_token));
	t_ms_token			*token;

	token = first;
	while (true)
	{
		if (token == NULL)
			break ;
		token -> next = NULL;
		string = populate_token(token, string);
		if (string == NULL)
			break ;
		if (*string == '\0')
			return (first);
		token -> next = malloc(sizeof(t_ms_token));
		token = token -> next;
	}
	ms_free_tokens(first);
	return (NULL);
}

#define BRANCHES_AND				0b000111110111
#define BRANCHES_OR					0b000111110111
#define BRANCHES_PIPE				0b000111110111
#define BRANCHES_DELIM				0b000000000111
#define BRANCHES_APPEND				0b000000000111
#define BRANCHES_INPUT				0b000000000111
#define BRANCHES_OUTPUT				0b000000000111
#define BRANCHES_OPEN				0b000111110111
#define BRANCHES_CLOSE				0b111000001000
#define BRANCHES_STRING_OPAQUE		0b111111101111
#define BRANCHES_STRING_SEMI		0b111111101111
#define BRANCHES_STRING_TRANSPARENT	0b111111101111

t_ms_token	*ms_check_syntax(t_ms_token *token)

{
	uint16_t const	token_branches[MS_TOKEN_MAX] = {
		BRANCHES_AND, BRANCHES_OR, BRANCHES_PIPE, BRANCHES_DELIM,
		BRANCHES_APPEND, BRANCHES_INPUT, BRANCHES_OUTPUT, BRANCHES_OPEN,
		BRANCHES_CLOSE, BRANCHES_STRING_OPAQUE, BRANCHES_STRING_SEMI,
		BRANCHES_STRING_TRANSPARENT
	};
	t_ms_token		*next_token;
	int				bracket_level;

	bracket_level = 0;
	while (token != NULL)
	{
		next_token = token -> next;
		if (next_token != NULL && next_token -> index == MS_TOKEN_SPACE)
			next_token = next_token -> next;
		if (token -> index != MS_TOKEN_SPACE && next_token != NULL
			&& !(token_branches[token -> index]
				& (0x800 >> next_token -> index)))
			return (token);
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = next_token;
	}
	return (NULL);
}

bool	ms_contains_index(t_ms_token *token, int8_t index)
{
	while (token != NULL)
	{
		if (token -> index == index)
			break ;
		token = token -> next;
	}
	return (token != NULL);
}

int8_t	next_operator_in_level(t_ms_token *token)
{
	int	bracket_level;

	bracket_level = 0;
	token = token -> next;
	while (token != NULL && bracket_level >= 0)
	{
		if (bracket_level == 0 && (token -> index == MS_TOKEN_AND
				|| token -> index == MS_TOKEN_OR
				|| token -> index == MS_TOKEN_PIPE))
			return (token -> index);
		bracket_level += (token -> index == MS_TOKEN_OPEN);
		bracket_level -= (token -> index == MS_TOKEN_CLOSE);
		token = token -> next;
	}
	return (MS_TOKEN_SPACE);
}

bool	ms_insert_token(t_ms_token *predecessor, int8_t index)
{
	t_ms_token *const	token = malloc(sizeof(t_ms_token));

	if (token == NULL)
		return (false);
	token -> next = predecessor -> next;
	token -> index = index;
	predecessor -> next = token;
	return (true);
}

t_ms_token	*ms_expand_precedence(t_ms_token *token, int8_t index)
{
	bool	in_brackets;

	in_brackets = false;
	while (token != NULL && token -> index != MS_TOKEN_CLOSE)
	{
		if (!in_brackets && token -> index == index || token -> index == MS_TOKEN_OPEN
			&& next_operator_in_level(token) > index)
		{
			if (!ms_insert_token(token, MS_TOKEN_OPEN))
				return (NULL);
			in_brackets = true;
			token = token -> next;
		}
		if (in_brackets && token -> next -> index == index || token -> next -> index == MS_TOKEN_CLOSE)
		{
			if (!ms_insert_token(token, MS_TOKEN_CLOSE))
				return (NULL);
			in_brackets = false;
			token = token -> next;
		}
		token = token -> next;
		if (token -> index == MS_TOKEN_OPEN)
		{
			token = ms_expand_precedence(token, index);
			if (token == NULL)
				return (NULL);
			if (token -> next == NULL)
				return (token);
			token = token -> next;
		}
	}
	return (token);
}

const char* token_names[MS_TOKEN_MAX + 1] = {
	" ", "&&", "||", "|", "<<", ">>", "<", ">", "(", ")"
};

void print_tokens(t_ms_token* token) {
	while(token != NULL) {
		if (token -> index < MS_TOKEN_STRING_OPAQUE)
			printf("%s", token_names[token -> index + 1]);
		else
			printf("STR:\"%s\"", token -> string);
		token = token -> next;
	}
	printf("\n");
}

t_ms_sequence	*ms_parse(char *string)
{
	t_ms_token	*first;
	t_ms_token	*token;

	first = ms_tokenize(string);
	if (first == NULL)
		return (NULL);
	token = ms_check_syntax(first);
	if (token == NULL)
		return (NULL);
	if (ms_contains_token(first, MS_TOKEN_AND)
		&& ms_expand_precedence(first, MS_TOKEN_AND) == NULL)
		return (NULL);
	if (ms_contains_token(first, MS_TOKEN_OR)
		&& ms_expand_precedence(first, MS_TOKEN_OR) == NULL)
		return (NULL);
	return (ms_process_tokens(first));
}

int main() {
	char buffer[1000];
	ft_bzero(buffer, 1000);
	write(1, "> ", 2);
	while(read(0, buffer, 1000) > 0) {
		t_ms_token* first = ms_tokenize(buffer);
		if(first == NULL) {
			puts("Tokenization failed");
			write(1, "> ", 2);
			continue;
		}
		{
			t_ms_token* token = malloc(sizeof(t_ms_token));
			token -> next = first;
			token -> index = MS_TOKEN_OPEN;
			first = token;
			token = first;
			while(token -> next != NULL)
				token = token -> next;
			token -> next = malloc(sizeof(t_ms_token));
			token -> next -> next = NULL;
			token -> next -> index = MS_TOKEN_CLOSE;
		}
		t_ms_token* pof = ms_check_syntax(first);
		if(pof != NULL) {
			printf("Syntax error: %s after %s\n", token_names[pof -> next -> index + 1], token_names[pof -> index + 1]);
			write(1, "> ", 2);
			continue;
		}
		ft_bzero(buffer, 1000);
		print_tokens(first);
		ms_expand_precedence(first -> next, MS_TOKEN_AND);
		print_tokens(first);
		ms_free_tokens(first);
		write(1, "> ", 2);
	}
}