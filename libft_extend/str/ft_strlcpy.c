/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:18:29 by reriebsc          #+#    #+#             */
/*   Updated: 2025/04/29 11:18:43 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	x;

	i = ft_strlen(src);
	x = 0;
	if (dstsize > 0)
	{
		while (x < (dstsize - 1) && src[x] != '\0')
		{
			dst[x] = src[x];
			x++;
		}
		dst[x] = '\0';
	}
	return (i);
}
