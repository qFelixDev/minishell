/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:29:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 17:52:22 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_getenv(char *name)
{
	t_list		*env_head;
	t_dict_env	*env;

	env_head = ms_minishell_get()->env;
	while (env_head)
	{
		env = env_head->content;
		if (ft_strcmp(name, env->key) == 0)
			return (env->value);
		env_head = env_head->next;
	}
	return (NULL);
}

char	**ms_gen_env(void)
{
	t_list	*cu;
	char	**env;
	int		i;
	char	*tmp;

	env = gc_malloc(sizeof(char *) * (ft_lstsize(ms_minishell_get()->env) + 1));
	if (!env)
		exit(127);
	cu = ms_minishell_get()->env;
	i = -1;
	while (cu)
	{
		if (!((t_dict_env *)cu->content)->value)
		{
			cu = cu->next;
			continue ;
		}
		tmp = gc_add(ft_strjoin(((t_dict_env *)cu->content)->key, "="));
		if (!tmp)
			exit(127);
		env[++i] = gc_add(ft_strjoin(tmp, ((t_dict_env *)cu->content)->value));
		gc_free_ptr(tmp);
		cu = cu->next;
	}
	return (env[i] = NULL, env);
}

void	free_env_entry(void *content)
{
	t_dict_env	*entry;

	entry = content;
	gc_free_ptr(entry->key);
	gc_free_ptr(entry->value);
	gc_free_ptr(entry);
}
