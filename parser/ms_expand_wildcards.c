/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wildcards.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:26:12 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/23 18:11:43 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stddef.h>
#include <dirent.h>

size_t	get_word_length(char *word)
{
	size_t	length;

	length = 0;
	while (word[length] != '\0' && word[length] != '\1' && word[length] != '/')
		length++;
	return (length);
}

bool	matches_pattern(char *pattern, char *string)
{
	size_t	length;

	if (pattern[0] != '.' && string[0] == '.')
		return (false);
	length = get_word_length(pattern);
	if (pattern[length] != '\1')
		return (ft_strncmp(pattern, string, length) == 0
			&& ft_strlen(string) == length);
	while (true)
	{
		if (ft_strncmp(pattern, string, length) != 0)
			return (false);
		pattern += length;
		string += length;
		while (*pattern == '\1')
			pattern++;
		length = get_word_length(pattern);
		if (pattern[length] != '\1')
			break ;
		while (ft_strncmp(pattern, string, length) != 0 && *string != '\0')
			string++;
	}
	while ((ft_strncmp(pattern, string, length) != 0
			|| ft_strlen(string) != length) && *string != '\0')
		string++;
	return (ft_strncmp(pattern, string, length) == 0);
}

size_t	enumerate_matches(char *pattern, char *path, char **matches);

size_t	add_match(char *pattern, char *path, char *name, char **matches)
{
	struct stat		status;
	const size_t	new_length = ft_strlen(path) + ft_strlen(name) + 1;
	char *const		new_path = gc_add(ft_calloc(new_length + 1, sizeof(char)));

	ft_strlcat(new_path, path, new_length);
	ft_strlcat(new_path, name, new_length);
	if (lstat(new_path, &status) == -1)
		return (0);
	if (ft_strchr(pattern, '/') == NULL)
	{
		if (matches != NULL)
			*matches = new_path;
	}
	else if (S_ISDIR(status.st_mode))
	{
		ft_strlcat(new_path, "/", new_length + 1);
		while (*pattern != '/')
			pattern++;
		while (*pattern == '/')
			pattern++;
		return (enumerate_matches(pattern, new_path, matches));
	}
	return (ft_strchr(pattern, '/') == NULL);
}

size_t	enumerate_matches(char *pattern, char *path, char **matches)
{
	DIR				*directory;
	struct dirent	*entry;
	size_t			match_count;

	if (path[0] == '\0')
		directory = opendir("./");
	else
		directory = opendir(path);
	if (directory == NULL)
		return (0);
	match_count = 0;
	entry = readdir(directory);
	while (entry != NULL)
	{
		if (matches_pattern(pattern, entry -> d_name))
			match_count += add_match(pattern, path, entry -> d_name, matches
					+ (matches != NULL) * match_count);
		entry = readdir(directory);
	}
	closedir(directory);
	gc_free_ptr(path);
	return (match_count);
}

size_t	get_pattern(t_ms_token *token, char *pattern)
{
	const uint32_t	concatenation = token -> concatenation;
	size_t			length;

	length = 0;
	while (token != NULL && token -> concatenation == concatenation)
	{
		if (token -> index == MS_TOKEN_STRING && pattern != NULL)
			ft_strlcpy(pattern + length, token -> content,
				ft_strlen(token -> content) + 1);
		if (token -> index == MS_TOKEN_WILDCARD && pattern != NULL)
			pattern[length] = '\1';
		if (token -> index == MS_TOKEN_STRING)
			length += ft_strlen(token -> content);
		length += (token -> index == MS_TOKEN_WILDCARD);
		token = token -> next;
	}
	return (length);
}
