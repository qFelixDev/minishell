/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_v1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:02:40 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 00:09:26 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*join_str_array(char **list, int size)
{
	char	*result;
	size_t	len;
	int		i;

	len = 0;
	i = 0;
	while (i < size)
		len += ft_strlen(list[i++]) + 1;
	result = (char *)gc_add(ft_calloc(len + 1, sizeof(char)));
	if (!result)
		return (NULL);
	i = 0;
	while (i < size)
	{
		ft_strlcat(result, list[i], len + 1);
		if (i++ < size - 1)
			ft_strlcat(result, " ", len + 1);
	}
	return (result);
}

void	ft_free_cluster(char **cluster)
{
	int	i;

	if (cluster == NULL)
		return ;
	i = -1;
	while (cluster[++i])
		free(cluster[i]);
	free(cluster);
}

bool	ms_isspace(char character)
{
	return (character == 32 || (character >= 9 && character <= 13));
}

size_t	ms_deflate_string(char *string, char *deflated)
{
	size_t	length;

	length = 0;
	while (ms_isspace(*string))
		string++;
	while (*string != '\0')
	{
		while (!ms_isspace(*string) && *string != '\0')
		{
			if (deflated != NULL)
				deflated[length] = *string;
			length++;
			string++;
		}
		if (*string == '\0')
			break ;
		if (deflated != NULL)
			deflated[length] = ' ';
		length++;
		while (ms_isspace(*string))
			string++;
	}
	if (deflated != NULL)
		deflated[length] = '\0';
	return (length);
}
