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

char	*get_next_seperator(char *string, char *seperator)
{
	int	level;

	while (*string != ')')
	{
		if (strncmp(string, seperator) == 0)
			return (string);
		if (*string == '(')
	}
}

t_ms_string_sequence	*ms_sequence_by(char *string, char *sequencer)
{
	t_ms_string_sequence	*sequence;

	sequence = NULL;
}

void	ms_expand(char *command, char *expanded)
{

}