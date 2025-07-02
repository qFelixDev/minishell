/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:42:47 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 13:29:03 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_escaped(const char *str, size_t i)
{
	size_t	backslashes;

	backslashes = 0;
	while (i > 0 && str[i - 1] == '\\')
	{
		backslashes++;
		i--;
	}
	return (backslashes % 2);
}

static void	init_vars(int *exit_code, t_list **str_list, size_t *i)
{
	*exit_code = 0;
	*get_char_count() = 0;
	*str_list = NULL;
	*i = 0;
}

t_token_type	is_redirect(const char *str, size_t i)
{
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i] == '\\')
		return (MS_TOKEN_NONE);
	if (ft_strncmp(str + i, "<<", 2) == 0)
		return (MS_TOKEN_REDIRECT_INPUT_APPEND);
	if (ft_strncmp(str + i, ">>", 2) == 0)
		return (MS_TOKEN_REDIRECT_APPEND);
	if (*(str + i) == '<')
		return (MS_TOKEN_REDIRECT_INPUT);
	if (*(str + i) == '>')
		return (MS_TOKEN_REDIRECT_OUTPUT);
	return (MS_TOKEN_NONE);
}

char	*filter_and_get_redirects(t_ms_command *input, t_list **redirects,
		int *exit_code)
{
	size_t			i;
	t_token_type	type;
	t_list			*str_list;
	const size_t	input_len = ft_strlen(input); //len count muss überarbeitet werden wegen array of strings 

	init_vars(exit_code, &str_list, &i);
	while (i < input_len)
	{
		if (skip_safe_quotes(input, &i, &str_list))
			continue ;
		type = is_redirect(input, i);
		if (type == MS_TOKEN_NONE || is_escaped(input, i))
			add_str_to_result(input, &i, &str_list, 1);
		else if (i < input_len)
		{
			if (!parse_redirect(redirects, input, type, &i))
			{
				*exit_code = 258;
				break ;
			}
		}
	}
	return (strlst_to_str(str_list));
}
