/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_get_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:54:37 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/20 15:57:09 by ghodges          ###   ########.fr       */
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

void	expand_variables(t_ms_token *token)
{
	char	*value;

	while (token != NULL)
	{
		if (token -> index == MS_TOKEN_VARIABLE)
		{
			if (token -> content[0] == '?')
				value = gc_add(ft_itoa(ms_minishell_get() -> exit_status));
			else
			{
				value = ft_getenv(token -> content);
				if (value == NULL)
					value = "";
				value = gc_add(ft_strdup(value));
			}
			gc_free_ptr(token -> content);
			token -> content = value;
			token -> index = MS_TOKEN_STRING;
		}
		token = token -> next;
	}
}

t_ms_command	*ms_get_command(t_ms_token *first)
{
	t_ms_command	*command;
	t_ms_token		*token;

	token = first;
	while (token != NULL)
	{
		if (token -> index == MS_TOKEN_DELIM)
			ruin_delimiter(token -> next);
		token = token -> next;
	}
	expand_variables(token);
	if (!check_ambiguity(token))
		return (NULL);
	command = allocate_command(token);
	populate_command(command);
	return (command);
}
