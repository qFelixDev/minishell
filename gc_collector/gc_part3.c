/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_part3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:15:56 by ghodges           #+#    #+#             */
/*   Updated: 2025/07/23 17:56:20 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*gc_malloc(size_t size)
{
	return (gc_add(malloc(size)));
}

void	*gc_add_list_node(t_list *list)
{
	gc_add(list -> content);
	return (gc_add(list));
}
