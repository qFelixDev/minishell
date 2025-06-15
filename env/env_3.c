/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:29:09 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/15 12:29:46 by reriebsc         ###   ########.fr       */
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
