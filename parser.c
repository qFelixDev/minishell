enum {
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
	string += (*string == quote);
	return (string);
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

size_t	get_expanded_length(char *string)
{
	size_t	length;
	size_t	index;

	length = 2;
	while (*string != '\0')
	{
		if (strncmp(string, "&&", 2) == 0)
			length += 2;
		else if (strncmp(string, "|", 2) == 0)
			length += 2;
		length++;
	}
	return (length);
}

size_t	get_expanded_length(char *string)
{
	size_t	length;

	length = 2;
	while (*string != '\0')
	{
		length += (strncmp(string, "&&", 2) == 0) * 2;
		length += (*string == '|') * 2;
		string++;
	}
	return (length);
}

char	*ms_expand(char *command_string)
{
	char	*expanded_string;
	size_t	length;
	size_t	index;

	length = 2;
	index = 0;
	while (command_string[index] != '\0')
	{
		length += (command_string[index] == '&');
		length += (command_string[index] == '|') * 2;
		length++;
		index++;
	}
	expanded_string = malloc(length + 1);
	if (expanded_string == NULL)
		return (NULL);
	ms_expand_recursive(command_string, expanded_string);
}
