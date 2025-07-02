/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_part2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:38:42 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 11:20:54 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_redirect(void *content)
{
	t_redirect	*redirect;

	redirect = content;
	gc_free_ptr(redirect->file);
	gc_free_ptr(redirect);
}

static t_list	**get_gc_head_files(void)
{
	static t_list	*gc_head;

	gc_head = NULL;
	return (&gc_head);
}

void	gc_close_fds(void)
{
	t_list	*current;
	t_list	*next;

	current = *get_gc_head_files();
	while (current)
	{
		next = current->next;
		close(*((int *)current->content));
		gc_free_ptr(current->content);
		gc_free_ptr(current);
		current = next;
	}
}

int	gc_add_fd(int fd)
{
	t_list	*new_node;
	int		*fd_ptr;

	if (fd < 0)
		return (-1);
	fd_ptr = gc_malloc(sizeof(int));
	*fd_ptr = fd;
	new_node = gc_add(ft_lstnew(fd_ptr));
	ft_lstadd_back(get_gc_head_files(), new_node);
	return (fd);
}

void	gc_close_fd(int fd)
{
	t_list	*current;
	t_list	*previous;

	if (fd < 0)
		return ;
	current = *get_gc_head_files();
	previous = NULL;
	while (current)
	{
		if (*((int *)current->content) == fd)
		{
			if (previous)
				previous->next = current->next;
			else
				*get_gc_head_files() = current->next;
			close(fd);
			gc_free_ptr(current->content);
			gc_free_ptr(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
	close(fd);
}
