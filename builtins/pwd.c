/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:07:04 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/05 13:57:36 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ms_pwd(void)
{
    char cwd[PATH_MAX];
	
	getcwd(cwd, PATH_MAX);
    printf("%s\n", cwd);
    free(cwd);
    return (0);
}
