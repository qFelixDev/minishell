/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:40:05 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 13:46:42 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	get_redirect_len(const char *str)
{
	const size_t	str_len = ft_strlen(str);
	size_t			len;

	len = 0;
	while (len < str_len)
	{
		if (skip_quotes(str, &len))
			continue ;
		if (ft_isspace(str[len]) && (len != 0 && str[len - 1] != '\\'))
			break ;
		if ((str[len] == '<' || str[len] == '>') && !is_escaped(str, len))
			break ;
		len++;
	}
	return (len);
}

static bool	parse_right_area(const char *str, size_t *i, t_redirect *redirect)
{
	size_t	filename_len;
	char	**split;

	if (str[*i] == '&')
	{
		(*i)++;
		redirect->fd_right = ft_atoi(str + *i);
		while (str[*i] && ft_isdigit(str[*i]))
			(*i)++;
		return (true);
	}
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
	filename_len = get_redirect_len(str + *i);
	if (filename_len == 0)
		return (false);
	redirect->file = gc_add(ft_substr(str, *i, filename_len));
	split = split_quotes(redirect->file);
	if (!split)
		return (gc_free_ptr(redirect->file), false);
	gc_free_ptr(redirect->file);
	redirect->file = split[0];
	ft_unescape_string(redirect->file);
	*i += filename_len;
	return (true);
}

bool	parse_redirect(t_list **redirects, const char *str,
		t_token_type redirect_type, size_t *i)
{
	t_redirect	*redirect;

	redirect = create_redirect(-1, -1, redirect_type, NULL);
	if (ft_isdigit(str[*i]))
	{
		redirect->fd_left = ft_atoi(str + *i);
		while (ft_isdigit(str[*i]))
			(*i)++;
	}
	(*i) += 1 + (redirect_type >= MS_TOKEN_REDIRECT_APPEND);
	if (str[*i] == 0 || (str[*i] == '&' && str[*i + 1] == 0))
		return (gc_free_ptr(redirect), false);
	if (!parse_right_area(str, i, redirect))
		return (gc_free_ptr(redirect), false);
	return (ft_lstadd_back(redirects, gc_add(ft_lstnew(redirect))), true);
}
