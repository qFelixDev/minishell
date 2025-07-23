/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_traverse_delims_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 18:34:26 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 18:51:18 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ms_traverse_delims(t_ms_sequence *sequence)
{
	size_t	obj_idx;

	obj_idx = 0;
	while (obj_idx < sequence -> object_count)
	{
		if (sequence -> is_sequence[obj_idx / 8] & (1u << (obj_idx % 8)))
			ms_traverse_delims(sequence -> objects[obj_idx]);
		else
			sequence -> delim_descriptors[obj_idx] = ms_read_delims(
					sequence -> objects[obj_idx], NULL);
		obj_idx++;
	}
}
