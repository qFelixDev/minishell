/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_enumerate_matches.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:27:54 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/27 00:06:35 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include<stdio.h>
#include <sys/stat.h>
#include "../libft_extend/libft.h"
#include <stdbool.h>

char	*next_wildcard(char *pattern)
{
	while (*pattern != '\1')
	{
		if (*pattern == '/' || *pattern == '\0')
			return (NULL);
		pattern++;
	}
	return (pattern);
}

bool	matches_pattern(char *pattern, char *string)
{
	char	*word_terminator;
	size_t	word_length;
	size_t	pattern_length;

	word_terminator = next_wildcard(pattern);
	if (word_terminator == NULL)
	{
		pattern_length = 0;
		while (pattern[pattern_length] != '/' && pattern[pattern_length] != '\0')
			pattern_length++;
		return (ft_strncmp(pattern, string, pattern_length) == 0);
	}
	word_length = word_terminator - pattern;
	while (true)
	{
		if (ft_strncmp(pattern, string, word_length) != 0)
			return (false);
		pattern += word_length;
		string += word_length;
		while (*pattern == '\1')
			pattern++;
		word_terminator = next_wildcard(pattern);
		if (word_terminator == NULL)
			break ;
		word_length = word_terminator - pattern;
		while (ft_strncmp(pattern, string, word_length) != 0 && *string != '\0')
			string++;
	}
	pattern_length = 0;
	while (pattern[pattern_length] != '/' && pattern[pattern_length] != '\0')
		pattern_length++;
	while (ft_strncmp(pattern, string, pattern_length) != 0 && *string != '\0')
		string++;
	return (ft_strncmp(pattern, string, pattern_length) == 0);
}

// Pass "/" to this function if path is absolute, "" if relative and the value of $HOME if preceded by a noodle

char	*next_pattern(char *pattern)
{
	while (*pattern != '/')
		pattern++;
	while (*pattern == '/')
		pattern++;
	return (pattern);
}

size_t	ms_enumerate_matches(char *pattern, char *path, char **matches, size_t offset)
{
	DIR *const		directory = opendir(path);
	struct dirent	*entry;
	struct stat		status;
	size_t			match_count;
	size_t			result;
	size_t			new_length;
	char			*new_path;

	match_count = 0;
	if (directory == NULL)
		return (SIZE_MAX);
	entry = readdir(directory);
	while (entry != NULL)
	{
		if (matches_pattern(pattern, entry -> d_name))
		{
			new_length = ft_strlen(path) + ft_strlen(entry -> d_name) + 1;
			new_path = ft_calloc(1, new_length + 1);
			if (new_path == NULL)
				return (closedir(directory), free(new_path), free(path), SIZE_MAX);
			ft_strlcat(new_path, path, new_length + 1);
			ft_strlcat(new_path, entry -> d_name, new_length + 1);
			if (lstat(new_path, &status) == -1)
				return (SIZE_MAX);
			if (ft_strchr(pattern, '/') == NULL)
			{
				if (matches != NULL)
					matches[offset + match_count] = new_path;
				match_count++;
			}
			else if (S_ISDIR(status.st_mode))
			{
				ft_strlcat(new_path, "/", new_length + 1);
				result = ms_enumerate_matches(next_pattern(pattern), new_path, matches, match_count);
				if (result == SIZE_MAX)
					return (closedir(directory), free(path), SIZE_MAX);
				match_count += result;
			}
		}
		entry = readdir(directory);
	}
	closedir(directory);
	free(path);
	return (match_count);
}

/*
int main() {
	char* pattern = "../lib\1\1\1t\1/\1l\1/\1";
	size_t amount = ms_enumerate_matches(pattern, ft_strdup("./"), NULL, 0);
	char** matches = malloc(amount * sizeof(char*));
	ms_enumerate_matches(pattern, ft_strdup("./"), matches, 0);
	size_t index = 0;
	while(index < amount) {
		puts(matches[index]);
		free(matches[index]);
		index++;
	}
	free(matches);
}
*/