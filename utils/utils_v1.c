/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_v1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:02:40 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/22 11:12:24 by reriebsc         ###   ########.fr       */
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
		return (pev(ERROR_MALLOC), NULL);
	i = 0;
	while (i < size)
	{
		ft_strlcat(result, list[i], len + 1);
		if (i++ < size - 1)
			ft_strlcat(result, " ", len + 1);
	}
	return (result);
}
