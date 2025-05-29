/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:21:40 by reriebsc          #+#    #+#             */
/*   Updated: 2025/04/29 11:21:47 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char		*str;

	str = s;
	str = str + ft_strlen(s);
	while (*str != (char)c && str != s)
		str--;
	if (*str == (char)c)
		return ((char *)str);
	return (NULL);
}
