/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:29:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/19 11:51:30 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	*ft_getenv(char *name)
{
	char		*str;
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
	return ("/usr/bin");
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

char	**ms_gen_env(void)
{
	t_list	*current;
	char	**env;
	int		i;
	char	*tmp;

	env = malloc(sizeof(char *) * (ft_lstsize(ms_minishell_get()->env) + 1));
	if (!env)
		exit(127);
	current = ms_minishell_get()->env;
	i = 0;
	while (current)
	{
		if (!((t_dict_env *)current->content)->value)
		{
			current = current->next;
			continue ;
		}
		tmp = ft_strjoin(((t_dict_env *)current->content)->key, "=");
		if (!tmp)
			exit(127);
		env[i] = ft_strjoin(tmp, ((t_dict_env *)current->content)->value);
		free(tmp);
		current = current->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

//int	main(int ac, char **av, char **env)
//{
//	if (!ms_generate_env(env))
//		return (1);
//	//ms_print_env();
//	//ms_cd("../includes");
//	//ms_print_env();
//
//	//printf("\n\n\n");
//
//	char *str;
//	char *key;
//
//	key = "PATH";
//	str = ft_getenv(key);
//	printf("%s", str);
//	return (0);
//}
