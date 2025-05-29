/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:12:41 by reriebsc          #+#    #+#             */
/*   Updated: 2025/04/29 11:12:56 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static char	*ft_ssubstr(const char *s, size_t len)
{
	char	*sub;
	size_t	i;

	if (!s || len == 0)
		return (NULL);
	sub = (char *)malloc(len + 1);
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len && s[i])
	{
		sub[i] = s[i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

static int	ft_count_words(const char *str, char c)
{
	int		count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*str == c)
			in_word = 0;
		str++;
	}
	return (count);
}

static void	ft_free_all(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	**ft_split_v2(const char *s, char c, char **out)
{
	const char	*start;
	int			i;

	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			start = s;
			while (*s && *s != c)
				s++;
			out[i] = ft_ssubstr(start, s - start);
			if (!out[i++])
			{
				ft_free_all(out);
				return (NULL);
			}
		}
		else
			s++;
	}
	out[i] = NULL;
	return (out);
}

char	**ft_split(const char *s, char c)
{
	char	**out;
	int		words;

	if (!s)
		return (NULL);
	words = ft_count_words(s, c);
	out = (char **)malloc((words + 1) * sizeof(char *));
	if (!out)
		return (NULL);
	out[words] = NULL;
	return (ft_split_v2(s, c, out));
}
