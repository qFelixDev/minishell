/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_manager_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:00:10 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 15:03:34 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

static bool	matches_quote(char c, char quote_type)
{
	return (c == quote_type);
}

static bool	get_word_length_helper(const char *str, bool *in_quotes,
		char *quote_type, size_t *len)
{
	if (is_quote(str[*len]) && !*in_quotes && !is_escaped(str, *len))
	{
		*in_quotes = true;
		*quote_type = str[*len];
	}
	else if (matches_quote(str[*len], *quote_type) && *in_quotes
		&& !is_escaped(str, *len))
	{
		(*len)++;
		(*in_quotes) = false;
		*quote_type = 0;
		if (!str[*len] || (ft_isspace(str[*len]) && !*in_quotes))
			return (false);
		return (true);
	}
	if (ft_isspace(str[*len]) && !*in_quotes && !is_escaped(str, *len))
		return (false);
	(*len)++;
	return (true);
}

static size_t	get_word_length(char const *str)
{
	size_t	len;
	bool	in_quotes;
	char	quote_type;

	len = 0;
	in_quotes = false;
	quote_type = 0;
	while (str[len])
	{
		if (!get_word_length_helper(str, &in_quotes, &quote_type, &len))
			break ;
	}
	return (len);
}

size_t	count_words(char const *str)
{
	size_t	count;
	bool	in_quotes;
	char	quote_type;
	size_t	i;

	count = 0;
	in_quotes = false;
	quote_type = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]) && !in_quotes)
			(i)++;
		if (!str[i])
			break ;
		(count)++;
		count_word_helper(str, &i, &in_quotes, &quote_type);
	}
	return (count);
}
