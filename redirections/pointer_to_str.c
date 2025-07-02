/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pointer_to_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:48:12 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 15:03:08 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	convert_pointer_to_string(char *str, void *ptr)
{
	unsigned long int	nb;
	int					i;

	nb = 0;
	i = 0;
	nb = (unsigned long int)ptr;
	while (nb > 0)
	{
		str[i] = nb % 10 + '0';
		nb /= 10;
		i++;
	}
	str[i] = '\0';
}
