/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:31:52 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/03 12:30:42 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft_extend/libft.h"

typedef struct s_ms_token {
	struct s_ms_token	*next;
	int					index;
	char				*string;
}	t_ms_token;

bool	ms_isspace(char character)
{
	return ((character >= 9 && character <= 13) || character == 32);
}

char	*create_token_string(char delimiter, char *string)
{
	char	*token_string;
	size_t	length;

	length = 0;
	if (delimiter == '\0')
		while (ft_strchr("|<>()'\"", string[length]) == NULL
			&& ft_strncmp(string + length, "&&", 2) != 0)
			length++;
	else
	{
		while (string[length] != delimiter && string[length] != '\0')
			length++;
		if (string[length] == '\0')
			return (NULL);
	}
	token_string = malloc(length + 1);
	if (token_string == NULL)
		return (NULL);
	ft_strlcpy(token_string, string, length + 1);
	return (token_string);
}

#include<stdio.h>

t_ms_token	*create_token(char **string_pt)
{
	char const *const	token_strings[12]
		= {"&&", "||", "|", "<<", ">>", "<", ">", "(", ")", "'", "\"", ""};
	t_ms_token *const	token = malloc(sizeof(t_ms_token));
	size_t				length;

	if (token == NULL)
		return (NULL);
	token -> next = NULL;
	token -> index = -1;
	while (++token -> index < 11) {
		if (ft_strncmp(*string_pt, token_strings[token -> index],
				ft_strlen(token_strings[token -> index])) == 0)
			break ;
	}
	*string_pt += ft_strlen(token_strings[token -> index]);
	if (token -> index < 9)
		return (token);
	token -> string = create_token_string(*token_strings[token -> index], *string_pt);
	*string_pt += ft_strlen(token -> string);
	*string_pt += ft_strlen(token_strings[token -> index]);
	if (token -> string == NULL)
		return (free(token), NULL);
	return (token);
}

void	free_tokens(t_ms_token *token)
{
	t_ms_token	*temp;

	while (token != NULL)
	{
		temp = token;
		token = token -> next;
		if (token -> index >= 9)
			free(temp -> string);
		free(temp);
	}
}

t_ms_token	*ms_tokenize(char *string)
{
	t_ms_token	*first;
	t_ms_token	*token;

	while (ms_isspace(*string))
		string++;
	first = create_token(&string);
	if (first == NULL)
		return (free_tokens(first), NULL);
	token = first;
	while (*string != '\0')
	{
		token -> next = create_token(&string);
		token = token -> next;
		if (token == NULL)
			return (free_tokens(first), NULL);
		while (ms_isspace(*string))
			string++;
	}
	return (first);
}

int main(int argc, char** argv) {
	if(argc < 2)
		return 0;
	t_ms_token* first = ms_tokenize(argv[1]);
	t_ms_token* token = first;
	while(token != NULL) {
		switch(token -> index) {
			case 0:
				puts("MS_AND");
				break;
			case 1:
				puts("MS_OR");
				break;
			case 2:
				puts("MS_PIPE");
				break;
			case 3:
				puts("MS_DELIMITED");
				break;
			case 4:
				puts("MS_APPEND");
				break;
			case 5:
				puts("MS_INPUT");
				break;
			case 6:
				puts("MS_OUTPUT");
				break;
			case 7:
				puts("MS_OPEN");
				break;
			case 8:
				puts("MS_CLOSE");
				break;
			case 9:
				puts("MS_TIGHT_STRING");
				break;
			case 10:
				puts("MS_LOOSE_STRING");
				break;
			case 11:
				puts("MS_OPEN_STRING");
				break;
		}
		token = token -> next;
	}
	//free_tokens(first);
}