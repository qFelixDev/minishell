/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:17:28 by reriebsc          #+#    #+#             */
/*   Updated: 2025/04/29 11:17:44 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		s1_len;
	size_t	i;

	s1_len = ft_strlen(s1);
	i = -1;
	if (!s1 || !s2)
		return (NULL);
	str = (char *)malloc((sizeof (char)) * (s1_len + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	while (s1[++i])
		str[i] = s1[i];
	i = -1;
	while (s2[++i])
		str[s1_len + i] = s2[i];
	str[s1_len + i] = '\0';
	return (str);
}
