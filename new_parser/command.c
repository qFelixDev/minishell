/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:54:37 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/22 12:57:16 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ruin_delimiter(t_ms_token *token)
{
	char			*content;
	const uint32_t	concatenation = token -> concatenation;

	while (token -> concatenation == concatenation)
	{
		if (token -> index == MS_TOKEN_WILDCARD)
			token -> content = gc_add(ft_strdup("*"));
		if (token -> index == MS_TOKEN_VARIABLE)
		{
			content = gc_malloc(ft_strlen(token -> content) + 2);
			content[0] = '$';
			ft_strlcpy(content + 1, token -> content, ft_strlen(token -> content) + 1);
			gc_free_ptr(token -> content);
			token -> content = content;
		}
		token -> index = MS_TOKEN_STRING;
		token = token -> next;
	}
}

void	expand_variable(t_ms_token *token)
{
	char	*value;

	if (token -> content[0] == '\0' && (token -> next == NULL
			|| token -> concatenation != token -> next -> concatenation))
		value = gc_add(ft_strdup("$"));
	else if (token -> content[0] == '$')
		value = gc_add(ft_itoa(getpid()));
	else if (token -> content[0] == '?')
		value = gc_add(ft_itoa(ms_minishell_get() -> exit_status));
	else
	{
		value = ft_getenv(token -> content);
		if (value == NULL)
			value = "";
		value = gc_add(ft_strdup(value));
	}
}

int	count_arguments(t_ms_token *token)
{
	int			argument_count;
	uint32_t	concatenation;

	argument_count = -ms_count_index(token, MS_TOKEN_DELIM);
	argument_count -= ms_count_index(token, MS_TOKEN_APPEND);
	argument_count -= ms_count_index(token, MS_TOKEN_INPUT);
	argument_count -= ms_count_index(token, MS_TOKEN_OUTPUT);
	while (token != NULL)
	{
		concatenation = token -> concatenation;
		argument_count += ms_expand_wildcards(token -> index, NULL);
		while (token != NULL && token -> concatenation == concatenation)
			token = token -> next;
	}
	return (argument_count);
}

t_ms_command	*allocate_command(t_ms_token *token)
{
	t_ms_command *const	command = gc_add(ft_calloc(1, sizeof(t_ms_command)));
	const int			argument_count = count_arguments(token);
	int					redirect_index;

	command -> argv = gc_add(ft_calloc(1, sizeof(t_ms_command)));
	command -> redirects = gc_add(ft_calloc(4, sizeof(char **)));
	redirect_index = 0;
	while (redirect_index < 4)
	{
		command -> redirects[redirect_index] = gc_add(ft_calloc(
				ms_count_index(token, MS_TOKEN_DELIM + redirect_index) + 1,
				sizeof(char *)));
		redirect_index++;
	}
	return (command);
}

t_ms_command	*ms_get_command(t_ms_token *first)
{
	t_ms_command	*command;
	t_ms_token		*token;

	token = first;
	while (token != NULL)
	{
		if (token -> index >= MS_TOKEN_APPEND
			&& token -> index <= MS_TOKEN_OUTPUT
			&& ms_expand_wildcards(token -> next, NULL) != 1)
			return (fprintf(stderr, "Ambiguous redirect\n"), NULL);
		if (token -> index == MS_TOKEN_DELIM)
			ruin_delimiter(token -> next);
		if (token -> index == MS_TOKEN_VARIABLE)
			expand_variable(token);
		token = token -> next;
	}
	command = allocate_command(token);
	populate_command(command);
	return (command);
}
