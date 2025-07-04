/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wildcards.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 20:27:54 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/05 17:06:10 by ghodges          ###   ########.fr       */
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
	//printf("%zu %zu\n", ft_strlen(string), length);
	return (ft_strncmp(pattern, string, length) == 0);
}

//int main() {
//	printf("%d\n", matches_pattern("_parser", "_compile_parser.sh"));
//}

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

size_t	enumerate_matches(char *pattern, char *path, char **matches);

size_t	add_match(char *pattern, char *path, char *name, char **matches)
{
	struct stat		status;
	const size_t	new_length = ft_strlen(path) + ft_strlen(name) + 1;
	char *const		new_path = ft_calloc(1, new_length + 1);

	if (new_path == NULL)
		return (0);
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
	DIR *const		directory = opendir(path);
	struct dirent	*entry;
	size_t			match_count;

	match_count = 0;
	if (directory == NULL)
		return (0);
	entry = readdir(directory);
	while (entry != NULL)
	{
		if (matches_pattern(pattern, entry -> d_name))
			match_count += add_match(pattern, path, entry -> d_name, matches
				+ (matches != NULL) * match_count);
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

size_t	ms_expand_wildcards(t_ms_token *token, char **paths)
{
	size_t		match_count;
	bool		is_absolute;
	int			pattern_index;
	char *const	pattern = ft_calloc(1, get_pattern(token, NULL) + 1);

	assert(token -> index >= MS_TOKEN_WILDCARD);
	if (pattern == NULL)
		return (0);
	get_pattern(token, pattern);
	is_absolute = (pattern[0] == '/');
	//print_pattern(pattern);
	match_count = enumerate_matches(
		pattern + is_absolute, ft_strdup(&"/"[!is_absolute]), NULL); // Change when ./ is omitted
	if (paths == NULL)
		return (match_count + (match_count == 0));
	match_count = enumerate_matches(
		pattern + is_absolute, ft_strdup(&"/"[!is_absolute]), paths); // Change when ./ is omitted
	if (match_count != 0)
		return (free(pattern), match_count);
	pattern_index = -1;
	while (pattern[++pattern_index] != '\0')
		if (pattern[pattern_index] == '\1')
			pattern[pattern_index] = '*';
	*paths = pattern;
	return (1);
}
