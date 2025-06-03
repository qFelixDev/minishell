/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:31:52 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/03 10:57:02 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*enum {
	MS_OR,
	MS_AND,
	MS_PIPE
}

typedef struct s_ms_command {
	char	**argv; // [0] = "echo", [1] = "bananenkuchen"
	char	*input; // NULL
	char	*output; // NULL
	char	*delimited; // NULL
	char	*append; // NULL
}	t_ms_command;

typedef struct s_ms_sequence {
	uint8_t	behavior;
	uint8_t	*is_sequence;
	void	**objects;
}	t_ms_sequence;

typedef struct s_ms_string_sequence {
	uint8_t	*is_sequence;
	void	**objects;
}	t_ms_string_sequence;

// ls && ls || ls && ls
// (ls && (ls || ls) && ls)

char	*skip_brackets(char *string)
{
	string++;
	while (*string != ')' && *string != '\0')
	{
		if (*string == '(')
			string = skip_brackets(string);
		else if (*string == '"')
			string = skip_quotes(string, '"');
		else if (*string == '\'')
			string = skip_quotes(string, '\'');
		string++;
	}
	string += (*string == ')');
	return (string);
}

char	*skip_quotes(char *string, char quote)
{
	string++;
	while (*string != quote && *string != '\0')
		string++;
	if (*string == '\0')
		return (NULL);
	return (string + 1);
}

char	*get_next_seperator(char *string, char *seperator)
{
	while (*string != ')')
	{
		if (strncmp(string, seperator, strlen(seperator)) == 0)
			return (string);
		if (*string == '(')
			string = skip_brackets(string);
		else if (*string == '"')
			string = skip_quotes(string, '"');
		else if (*string == '\'')
			string = skip_quotes(string, '\'');
		string++;
	}
	return (NULL);
}

char	*create_token(t_ms_token *token, char *string)
{
	char const *const	tokens[9] =
		{"&&", "||", "|", "<<", ">>", "<", ">", "'", "\"", ""};
	size_t				length;

	token -> index = -1;
	while (++token -> index < 9)
	{
		if (strncmp(string, tokens[token -> index],
			strlen(tokens[token -> index])) == 0)
			break ;
	}
	string += strlen(tokens[token -> index]);
	if (token -> index < 7)
		return (string);
	length = 0;
	while (string[length] )
}*/

char	*create_token(t_ms_token *token, char *string)
{
	char const *const	tokens[12]
		= {"&&", "||", "|", "<<", ">>", "<", ">", "(", ")", "'", "\"", ""};
	size_t				length;

	token -> index = -1;
	while (++token -> index < 11)
		if (ft_strncmp(string, tokens[token -> index],
				ft_strlen(tokens[token -> index])) == 0)
			break ;
	string += ft_strlen(tokens[token -> index]);
	if (token -> index < 9)
		return (string);
	length = 0;
	if (token -> index < 11)
		while (string[length] != *tokens[token->index] && string[length] != 0)
			length++;
	else
		while (ft_strchr("|<>()'\"", string[length]) == NULL
			&& ft_strncmp(string[length], "&&", 2) != 0)
			length++;
	token -> string = malloc(length + 1, 1);
	if (token -> string 
	ft_strlcpy(token -> string, )
	return (string + length + ft_strlen(tokens[token -> index]));
}
