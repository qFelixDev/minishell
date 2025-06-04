/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:07:04 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/04 16:24:38 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ms_pwd(void)
{
    char cwd[PATH_MAX];
	
	getcwd(cwd, PATH_MAX);
    if (!cwd)
        return(perror("pwd"), 1);
    printf("%s\n", cwd);
    free(cwd);
    return (0);
}