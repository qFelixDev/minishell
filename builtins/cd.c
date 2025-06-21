/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:33:23 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/20 08:23:31 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//cd - handeln und cd ~ handeln 

static int	ms_cd(char *path)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));
	if (chdir(path) != 0)
		return (perror("Path Error"), 1);
	set_env_value("OLDPWD", cwd);
	getcwd(cwd, sizeof(cwd));
	set_env_value("PWD", cwd);
	return (0);
}

//int	ft_cd(t_command *command)
//{
//	char	*oldpwd;
//
//	if (command->argv[1] != NULL && command->argv[2] != NULL)
//		return (pec("cd : Too much argument"));
//	if (!command->argv[1])
//	{
//		if (!get_env_value("HOME") || !*get_env_value("HOME"))
//			return (pec("HOME not set"));
//		return (change_directory(get_env_value("HOME")));
//	}
//	if (ft_strncmp(command->argv[1], "-", 2) == 0
//		|| ft_strncmp(command->argv[1], "--", 3) == 0)
//	{
//		oldpwd = get_env_value("OLDPWD");
//		if (!oldpwd || !*oldpwd)
//			return (pec("OLDPWD not set"));
//		printf("%s\n", oldpwd);
//		return (change_directory(oldpwd));
//	}
//	return (change_directory(command->argv[1]));
//}