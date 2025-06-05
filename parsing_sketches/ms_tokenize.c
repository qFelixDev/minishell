/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:49:38 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/05 12:29:19 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include "libft_extend/libft.h"
#include "token.h"

bool	ms_isspace(char character)
{
	return ((character >= 9 && character <= 13) || character == 32);
}

static char	*populate_token_content(
	t_ms_token *token, char *string, char terminator)
{
	size_t	length;

	length = 0;
	if (terminator == '\0')
		while (!ms_isspace(string[length])
			&& ft_strncmp(string + length, "&&", 2) != 0
			&& ft_strchr("|<>()'\"", string[length]) == NULL)
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

static char	*populate_token(t_ms_token *token, char *string)
{
	char const *const	token_strings[MS_TOKEN_MAX]
		= {"&&", "||", "|", "<<", ">>", "<", ">", "(", ")", "'", "\"", ""};

	token -> next = NULL;
	token -> index = -1;
	while (++token -> index < MS_TOKEN_MAX)
		if (ft_strncmp(string, token_strings[token -> index],
				ft_strlen(token_strings[token -> index])) == 0)
			break ;
	string += ft_strlen(token_strings[token -> index]);
	if (token -> index < MS_TOKEN_STRING_OPAQUE)
		return (string);
	token -> content = NULL;
	token -> concatenate_content = false;
	string
		= populate_token_content(token, string, *token_strings[token -> index]);
	if (string == NULL)
		return (NULL);
	string += ft_strlen(token_strings[token -> index]);
	if (ms_isspace(*string))
		token -> concatenate_content = true;
	return (string);
}

t_ms_token	*ms_tokenize(char *string)
{
	t_ms_token *const	first = malloc(sizeof(t_ms_token));
	t_ms_token			*token;

	if (first == NULL)
		return (NULL);
	first -> next = NULL;
	first -> index = MS_TOKEN_OPEN;
	token = first;
	while (ms_isspace(*string))
		string++;
	while (*string != '\0')
	{
		token -> next = malloc(sizeof(t_ms_token));
		token = token -> next;
		if (token == NULL)
			return (ms_free_tokens(first), NULL);
		string = populate_token(token, string);
		if (string == NULL)
			return (ms_free_tokens(first), NULL);
		while (ms_isspace(*string))
			string++;
	}
	if (!ms_insert_token(token, MS_TOKEN_CLOSE))
		return (ms_free_tokens(first), NULL);
	return (first);
}

/*char const *const	token_names[MS_TOKEN_STRING_OPAQUE] = {
	"&&", "||", "|", "<<", ">>", "<", ">", "(", ")"
};

void print_tokens(t_ms_token* token) {
	while(token != NULL) {
		if (token -> index < MS_TOKEN_STRING_OPAQUE)
			printf("%s", token_names[token -> index]);
		else
			printf("\"%s\"", token -> content);
		token = token -> next;
	}
	printf("\n");
}*/

/*int main() {
	char buffer[1000];
	ft_bzero(buffer, 1000);
	write(1, "> ", 2);
	while(read(0, buffer, 1000) > 0) {
		t_ms_token* first = ms_tokenize(buffer);
		ft_bzero(buffer, 1000);
		if(first == NULL) {
			puts("Tokenization failed");
			write(1, "> ", 2);
			continue;
		}
		if (ms_check_syntax(first) != NULL) {
			puts("Invalid Syntax");
			write(1, "> ", 2);
			ms_free_tokens(first);
			continue ;
		}
		print_tokens(first);
		ms_expand_precedence(first, MS_TOKEN_AND);
		print_tokens(first);
		ms_expand_precedence(first, MS_TOKEN_OR);
		print_tokens(first);
		ms_free_tokens(first);
		write(1, "> ", 2);
	}
}*/