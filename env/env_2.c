/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:05:45 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 17:10:04 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	ms_set_env_value(const char *key, const char *value)
{
	t_dict_env	*node;

	if (!key)
		return (false);
	node = ms_get_env_node(key);
	if (!value)
	{
		if (node && node->value)
			return (true);
		return (ms_add_env_node(key, NULL));
	}
	if (node)
	{
		gc_free_ptr(node->value);
		node->value = gc_add(ft_strdup(value));
		return (true);
	}
	return (ms_add_env_node(key, value));
}

t_dict_env	*ms_get_env_node(const char *key)
{
	t_list	*gen_node;

	gen_node = ms_minishell_get()->env;
	while (gen_node)
	{
		if (ft_strcmp(((t_dict_env *)gen_node->content)->key, (char *)key) == 0)
		{
			return ((t_dict_env *)gen_node->content);
		}
		gen_node = gen_node->next;
	}
	return (NULL);
}

void	ms_open_shells(void)
{
	t_dict_env	*node;
	char		*str;
	int			numb_shells;

	node = ms_get_env_node("SHLVL");
	if (!node)
	{
		ms_add_env_dict("SHLVL=1");
		return ;
	}
	numb_shells = ft_atoi(node->value);
	numb_shells += 1;
	str = gc_add(ft_itoa(numb_shells));
	gc_free_ptr(node->value);
	node->value = str;
}

bool	is_valid_identifier(const char *identifier)
{
	size_t	i;

	if (!identifier || *identifier == '\0')
		return (false);
	if (!ft_isalpha(identifier[0]) && identifier[0] != '_')
		return (false);
	i = 1;
	while (identifier[i] != '\0')
	{
		if (!ft_isalnum(identifier[i]) && identifier[i] != '_')
			return (false);
		i++;
	}
	return (true);
}
