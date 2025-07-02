/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:56:10 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 15:03:29 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_escaped(const char *str, size_t i)
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

bool	skip_quotes(const char *str, size_t *i)
{
	if (str[*i] == '\'' && !is_escaped(str, *i))
	{
		(*i)++;
		while (str[*i] && (str[*i] != '\'' || is_escaped(str, *i)))
			(*i)++;
		(*i)++;
		return (true);
	}
	if (str[*i] == '\"' && !is_escaped(str, *i))
	{
		(*i)++;
		while (str[*i] && (str[*i] != '\"' || is_escaped(str, *i)))
			(*i)++;
		(*i)++;
		return (true);
	}
	return (false);
}

char	**split_quotes(char const *str)
{
	char	**result;
	size_t	word_count;
	size_t	i;
	size_t	word_len;

	if (!str)
		return (NULL);
	word_count = count_words(str);
	if (word_count == 0)
		return (gc_add(ft_calloc(2, sizeof(char *))));
	result = gc_malloc(sizeof(char *) * (word_count + 1));
	i = 0;
	while (i < word_count)
	{
		while (ft_isspace(*str))
			str++;
		word_len = get_word_length(str);
		result[i] = extract_word(str, word_len);
		str += word_len;
		i++;
	}
	result[i] = NULL;
	return (result);
}
