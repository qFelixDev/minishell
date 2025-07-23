/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:52:36 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 17:10:42 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ms_split_key_value(const char *str, char *key, char *value)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (str[x] != '=' && str[x] != '\0')
	{
		key[x] = str[x];
		++x;
	}
	key[x] = '\0';
	x += 1;
	while (str[x])
	{
		value[y] = str[x];
		++x;
		++y;
	}
	value[y] = '\0';
}

t_minishell	*ms_minishell_get(void)
{
	static t_minishell	minishell;

	return (&minishell);
}

int	ms_add_env_node(const char *key, const char *value)
{
	t_dict_env	*env_node;
	t_list		*new_node;

	env_node = gc_malloc(sizeof(t_dict_env));
	new_node = gc_add_list_node(ft_lstnew(env_node));
	env_node->key = gc_add(ft_strdup(key));
	if (value)
		env_node->value = gc_add(ft_strdup(value));
	else
		env_node->value = NULL;
	ft_lstadd_back(&ms_minishell_get()->env, new_node);
	return (1);
}

int	ms_add_env_dict(const char *str)
{
	char	key[PATH_MAX];
	char	value[PATH_MAX];

	ms_split_key_value(str, key, value);
	return (ms_add_env_node(key, value));
}

int	ms_generate_env(char **env)
{
	int		x;
	char	cwd[PATH_MAX];

	x = 0;
	while (env[x] != NULL)
	{
		if (!ms_add_env_dict(env[x]))
			return (0);
		++x;
	}
	ms_open_shells();
	ms_set_env_value("OLDPWD", NULL);
	if (getcwd(cwd, PATH_MAX))
		ms_set_env_value("PWD", cwd);
	return (1);
}
