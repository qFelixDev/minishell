/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wildcards.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:27:54 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/30 17:50:02 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include<stdio.h>
#include <sys/stat.h>
#include "../libft_extend/libft.h"
#include <stdbool.h>
#include "../includes/minishell.h"
#include<assert.h>

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

bool	matches_pattern_old(char *pattern, char *string)
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

	length = get_word_length(pattern);
	if (pattern[length] == '\1')
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
	while (ft_strncmp(pattern, string, length) != 0 && *string != '\0')
		string++;
	return (ft_strncmp(pattern, string, length) == 0);
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

size_t	enumerate_matches(char *pattern, char *path, char **matches, size_t offset)
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
				result = enumerate_matches(next_pattern(pattern), new_path, matches, match_count);
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

size_t	get_pattern(t_ms_token *token, char *pattern)
{
	size_t	length;

	length = 0;
	while (token != NULL && token -> index >= MS_TOKEN_WILDCARD)
	{
		if (token -> index == MS_TOKEN_STRING && pattern != NULL)
			ft_strlcpy(pattern + length, token -> content,
				ft_strlen(token -> content) + 1);
		if (token -> index == MS_TOKEN_WILDCARD && pattern != NULL)
			pattern[length] = '\1';
		if (token -> index == MS_TOKEN_STRING)
			length += ft_strlen(token -> content);
		length += (token -> index == MS_TOKEN_WILDCARD);
		if (!token -> concatenate_content)
			break ;
		token = token -> next;
	}
	return (length);
}

void print_pattern(char *pattern) {
	while(*pattern != '\0') {
		if(*pattern == '\1')
			printf("[*]");
		else
			putchar(*pattern);
		pattern++;
	}
	printf("\n");
}

size_t	ms_expand_wildcards(t_ms_token *token, char **paths)
{
	size_t		match_count;
	char *const	pattern = ft_calloc(1, get_pattern(token, NULL) + 1);

	assert(token -> index >= MS_TOKEN_WILDCARD);
	if (pattern == NULL)
		return (SIZE_MAX);
	get_pattern(token, pattern);
	print_pattern(pattern);
	match_count = enumerate_matches(pattern, ft_strdup("./"), NULL, 0);
	if (match_count == SIZE_MAX || paths == NULL)
		return (match_count);
	match_count = enumerate_matches(pattern, ft_strdup("./"), paths, 0);
	free(pattern);
	return (match_count);
}
