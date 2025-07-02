/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unscaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:04:18 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 15:05:17 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_backslash(char *str, int *i, int *j, int len)
{
	if (str[*i] == '\\' && *i + 1 < len)
	{
		if (str[*i + 1] == 'n' && !is_escaped(str, *i))
		{
			str[(*j)++] = '\n';
			*i += 2;
		}
		else if (str[*i + 1] == '\\')
		{
			str[(*j)++] = '\\';
			*i += 2;
		}
		else
		{
			str[(*j)++] = str[*i + 1];
			*i += 2;
		}
	}
	else
		str[(*j)++] = str[(*i)++];
}

static void	process_quote_char(t_escape_data data)
{
	if (!*data.in_quotes)
	{
		*data.in_quotes = true;
		*data.quote_type = data.str[*data.i];
		data.str[(*data.j)++] = data.str[(*data.i)++];
	}
	else if (data.str[*data.i] == *data.quote_type)
	{
		*data.in_quotes = false;
		data.str[(*data.j)++] = data.str[(*data.i)++];
	}
	else
		data.str[(*data.j)++] = data.str[(*data.i)++];
}

static void	handle_quotes(t_escape_data data)
{
	if ((data.str[*data.i] == '"' || data.str[*data.i] == '\'') && (*data.i == 0
			|| data.str[*data.i - 1] != '\\'))
		process_quote_char(data);
	else
		handle_backslash(data.str, data.i, data.j, data.len);
}

char	*ft_unescape_string(char *str)
{
	int		i;
	int		j;
	int		len;
	char	quote_type;
	bool	in_quotes;

	if (!str)
		return (NULL);
	len = strlen(str);
	i = 0;
	j = 0;
	in_quotes = false;
	quote_type = 0;
	while (i < len)
		handle_quotes((t_escape_data){str, &i, &j, &in_quotes, &quote_type,
			len});
	str[j] = '\0';
	return (str);
}
