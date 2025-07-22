/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:33:23 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/23 00:28:37 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ms_cd(char *path)
{
	char	cwd[PATH_MAX];

	if (path == NULL)
	{
		path = ft_getenv("HOME");
		if (path == NULL)
			path = "/";
	}
	if (ft_strcmp(path, "-") == 0)
	{
		path = ft_getenv("OLDPWD");
		printf("%s\n", path);
	}
	if (ft_strcmp(path, "--") == 0)
		path = ft_getenv("HOME");
	getcwd(cwd, sizeof(cwd));
	if (chdir(path) != 0)
		return (perror("Path Error"), 1);
	ms_set_env_value("OLDPWD", cwd);
	getcwd(cwd, sizeof(cwd));
	ms_set_env_value("PWD", cwd);
	return (0);
}
