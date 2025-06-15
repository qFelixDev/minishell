/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:52:36 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/15 12:28:18 by reriebsc         ###   ########.fr       */
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

	env_node = malloc(sizeof(t_dict_env));
	new_node = ft_lstnew(env_node);
	env_node->key = ft_strdup(key);
	if (value)
		env_node->value = ft_strdup(value);
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

void	ms_print_env(void)
{
	t_dict_env	*node;
	t_list		*env;

	env = ms_minishell_get()->env;
	while (env)
	{
		node = env->content;
		if (env->next && !node->value)
		{
			env = env->next;
			continue ;
		}
		printf("%s", node->key);
		printf("=%s\n", node->value);
		env = env->next;
	}
}

static int	ms_cd(char *path)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));
	if (chdir(path) != 0)
		return (perror("Path Error"), 1);
	ms_set_env_value("OLDPWD", cwd);
	getcwd(cwd, sizeof(cwd));
	ms_set_env_value("PWD", cwd);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	if (!ms_generate_env(env))
		return (1);
	ms_print_env();
	ms_cd("../includes");
	ms_print_env();
	return (0);
}