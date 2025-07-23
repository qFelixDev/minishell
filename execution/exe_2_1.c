/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_2_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:19:58 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 20:19:56 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/wait.h>

bool	handle_intermediate_redirects(char **redirects, size_t *last_index)
{
	int		descriptor;
	size_t	i;

	i = 0;
	while (redirects[*last_index + 1] != NULL)
		(*last_index)++;
	while (i < *last_index)
	{
		descriptor = gc_add_fd(open(redirects[i++], O_CREAT | O_WRONLY, 0644));
		if (descriptor == -1)
			return (perror("open (dummy redirect)"), false);
		gc_close_fd(descriptor);
	}
	return (true);
}

int	get_target_fd(int type)
{
	if (type == 2)
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

int	get_open_flags(int type)
{
	if (type == 1)
		return (O_WRONLY | O_CREAT | O_APPEND);
	if (type == 2)
		return (O_RDONLY);
	if (type == 3)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	return (-1);
}

bool	open_redirect_fd(char *file, int type, int *fd_out)
{
	int	flags;

	flags = get_open_flags(type);
	if (flags == -1)
		return (false);
	*fd_out = gc_add_fd(open(file, flags, 0644));
	if (*fd_out == -1)
		return (perror("open (redirect)"), false);
	return (true);
}

bool	dup_and_close(int descriptor, int target_fd)
{
	if (dup2(descriptor, target_fd) == -1)
		return (perror("dup2 (redirect)"), gc_close_fd(descriptor), false);
	gc_close_fd(descriptor);
	return (true);
}
