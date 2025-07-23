/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wildcards_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 18:10:11 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 18:10:29 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ms_expand_wildcards(t_ms_token *token, char **paths)
{
	size_t		match_count;
	int			pattern_index;
	bool		is_absolute;
	char *const	pattern = gc_add(ft_calloc(1, get_pattern(token, NULL) + 1));

	get_pattern(token, pattern);
	is_absolute = (pattern[0] == '/');
	match_count = enumerate_matches(
			pattern + is_absolute, gc_add(ft_strdup(&"/"[!is_absolute])), NULL);
	if (paths == NULL)
		return (gc_free_ptr(pattern), match_count + (match_count == 0));
	match_count = enumerate_matches(
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
