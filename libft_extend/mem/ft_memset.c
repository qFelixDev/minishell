/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:59:13 by reriebsc          #+#    #+#             */
/*   Updated: 2025/04/29 10:59:26 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memset(void *ptr, int value, size_t len)
{
	unsigned char	*c;
	unsigned int	x;

	c = (unsigned char *)ptr;
	x = 0;
	while (x < len)
	{
		c[x++] = value;
	}
	ptr = (void *)c;
	return (ptr);
}
