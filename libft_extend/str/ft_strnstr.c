/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:21:08 by reriebsc          #+#    #+#             */
/*   Updated: 2025/04/29 11:21:19 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strnstr(const char *haystack, const char *needl, size_t len)
{
	size_t	len2;

	if (*needl == '\0')
		return ((char *)haystack);
	len2 = ft_strlen(needl);
	while (*haystack != '\0' && len-- >= len2)
	{
		if (*haystack == *needl && ft_memcmp(haystack, needl, len2) == 0)
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
