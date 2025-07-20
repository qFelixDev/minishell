/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_expand_wildcards.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 17:26:12 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/20 11:32:08 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

size_t	get_pattern(t_ms_token *token, char *pattern)
{
	const uint32_t	concatenation = token -> concatenation;
	size_t			length;

	length = 0;
	if (token -> index == MS_TOKEN_HOME && token -> next -> concatenation
		!= concatenation ||
		(token -> next == MS_TOKEN_STRING && token -> content[0] == '/'))
	{
		if (pattern != NULL)
			ft_strlcpy(pattern + length, ft_getenv("HOME"),
				ft_strlen(ft_getenv("HOME")));
		length += ft_strlen(ft_getenv("HOME"));
	}
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

size_t	ms_expand_wildcards(t_ms_token *token, char **paths)
{
	char *const	pattern = gc_add(ft_calloc(1, get_pattern(token, NULL)));
}