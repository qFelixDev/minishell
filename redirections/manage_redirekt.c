/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_redirekt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:47:13 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 15:03:27 by reriebsc         ###   ########.fr       */
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

bool	is_redirect_token(t_token_type type)
{
	return (type == MS_TOKEN_REDIRECT_INPUT || type == MS_TOKEN_REDIRECT_OUTPUT
		|| type == MS_TOKEN_REDIRECT_APPEND
		|| type == MS_TOKEN_REDIRECT_INPUT_APPEND);
}

t_redirect	*create_redirect(int fd_left, int fd_right, t_token_type type,
		char *file)
{
	t_redirect	*redirect;

	redirect = gc_malloc(sizeof(t_redirect));
	redirect->fd_left = fd_left;
	redirect->fd_right = fd_right;
	redirect->type = type;
	redirect->file = file;
	return (redirect);
}
