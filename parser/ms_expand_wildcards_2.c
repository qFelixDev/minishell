/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wildcards_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 18:10:11 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 18:50:49 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ms_expand_wildcards(t_ms_token *token, char **paths)
{
	size_t		match_count;
	int			pattern_index;
	bool		is_absolute;
	char *const	pattern = gc_add(ft_calloc(1, ms_get_pattern(token, NULL) + 1));

	ms_get_pattern(token, pattern);
	is_absolute = (pattern[0] == '/');
	match_count = ms_enumerate_matches(
			pattern + is_absolute, gc_add(ft_strdup(&"/"[!is_absolute])), NULL);
	if (paths == NULL)
		return (gc_free_ptr(pattern), match_count + (match_count == 0));
	match_count = ms_enumerate_matches(
			pattern + is_absolute,
			gc_add(ft_strdup(&"/"[!is_absolute])), paths);
	if (match_count != 0)
		return (gc_free_ptr(pattern), match_count);
	pattern_index = -1;
	while (pattern[++pattern_index] != '\0')
		if (pattern[pattern_index] == '\1')
			pattern[pattern_index] = '*';
	*paths = pattern;
	return (1);
}

static bool	match_suffix(char *pattern, char *string, size_t length)
{
	while ((ft_strncmp(pattern, string, length) != 0
			|| ft_strlen(string) != length) && *string != '\0')
		string++;
	return (ft_strncmp(pattern, string, length) == 0);
}

static bool	match_prefix(char **pattern, char **string)
{
	size_t	length;

	length = get_word_length(*pattern);
	if ((*pattern)[length] != '\1')
		return (ft_strncmp(*pattern, *string, length) == 0
			&& ft_strlen(*string) == length);
	while (true)
	{
		if (ft_strncmp(*pattern, *string, length) != 0)
			return (false);
		*pattern += length;
		*string += length;
		while (**pattern == '\1')
			(*pattern)++;
		length = get_word_length(*pattern);
		if ((*pattern)[length] != '\1')
			break ;
		while (ft_strncmp(*pattern, *string, length) != 0 && **string != '\0')
			(*string)++;
	}
	return (match_suffix(*pattern, *string, length));
}

bool	matches_pattern(char *pattern, char *string)
{
	if (pattern[0] != '.' && string[0] == '.')
		return (false);
	return (match_prefix(&pattern, &string));
}
