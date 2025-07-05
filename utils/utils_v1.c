/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_v1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:02:40 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/05 14:23:50 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*join_str_array(char **list, int size)
{
	char	*result;
	size_t	len;
	int		i;

	len = 0;
	i = 0;
	while (i < size)
		len += ft_strlen(list[i++]) + 1;
	result = (char *)gc_add(ft_calloc(len + 1, sizeof(char)));
	if (!result)
		return (/*pev(ERROR_MALLOC), */NULL);
	i = 0;
	while (i < size)
	{
		ft_strlcat(result, list[i], len + 1);
		if (i++ < size - 1)
			ft_strlcat(result, " ", len + 1);
	}
	return (result);
}

void	ft_free_cluster(char **cluster)
{
	int	i;

	i = -1;
	while (cluster[++i])
		free(cluster[i]);
	free(cluster);
}

//void	free_ast_node(void *content)
//{
//	t_ms_sequence	*node;
//
//	node = content;
//	if (node->left)
//		free_ast_node(node->left);
//	if (node->right)
//		free_ast_node(node->right);
//	gc_free_ptr(node->value);
//	node->value = NULL;
//	if (node->heredoc_filename)
//	{
//		unlink(node->heredoc_filename);
//		gc_free_ptr(node->heredoc_filename);
//		node->heredoc_filename = NULL;
//	}
//	if (node->type == AST_COMMAND)
//	{
//		gc_list_clear(&node->redirects, free_redirect);
//		node->redirects = NULL;
//	}
//	gc_free_ptr(node);
//}
