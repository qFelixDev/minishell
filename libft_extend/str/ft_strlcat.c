/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:17:55 by reriebsc          #+#    #+#             */
/*   Updated: 2025/04/29 11:39:32 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	x;
	size_t	src_len;

	x = 0;
	dst_len = 0;
	src_len = ft_strlen(src);
	while (dst_len < dstsize && dst[dst_len] != '\0')
		dst_len++;
	if (dstsize <= dst_len)
		return (dstsize + src_len);
	while (dst_len + x < dstsize - 1 && src[x] != '\0')
	{
		dst[dst_len + x] = src[x];
		x++;
	}
	if (dst_len + x < dstsize)
		dst[dst_len + x] = '\0';
	return (dst_len + src_len);
}
