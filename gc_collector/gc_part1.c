/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_part1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:31:01 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/22 10:37:51 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_list	**get_gc_head(void)
{
	static t_list	*gc_head = NULL;

	return (&gc_head);
}

void	gc_free_ptr(void *addr)
{
	t_list	*current;
	t_list	*previous;

	current = *get_gc_head();
	previous = NULL;
	while (current)
	{
		if (current->content == addr)
		{
			if (previous)
				previous->next = current->next;
			else
				*get_gc_head() = current->next;
			free(current->content);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
	free(addr);
}

void	gc_list_clear(t_list **list, void (*del)(void *))
{
	t_list	*tmp;

	if (!list || !del)
		return ;
	while (*list)
	{
		tmp = *list;
		*list = (*list)->next;
		del(tmp->content);
		gc_free_ptr(tmp);
	}
}

void	gc_free(void)
{
	ft_lstclear(get_gc_head(), free);
	*get_gc_head() = NULL;
}

