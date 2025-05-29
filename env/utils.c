/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:58:45 by reriebsc          #+#    #+#             */
/*   Updated: 2025/05/29 13:25:26 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/enviroment.h"
#include <stdio.h>
#include <string.h>

void	ms_free_envc(char **envc)
{
	int	x;

	x = 0;
	if (!envc)
		return ;
	while (envc[x] != NULL)
	{
		free(envc[x]);
		++x;
	}
	free(envc);
}

char    **ms_env_gen(char **envp)
{
    int x;
    char    **envc;

    x = 0;
    while (envp[x])
        x++;
    envc = malloc(x + 1 * sizeof(char *));
    if (!envc)
        return (NULL);
    envc[x] = NULL;
    while (--x)
    {
        envc[x] = strdup(envp[x]); //ft_strdup(envp[x]);
        if (!envc[x])
        {
            ms_free_envc(envc);
            return (NULL);
        }
    }
    return (envc);    
}

void ms_print_env(char **envc)
{
    int i = 0;
    printf("Kopiertes Environment:\n");
    printf("env[%d]: %s\n", i, envc[i]);
    while (envc[i])
    {
        printf("Kopiertes Environment:\n");
        printf("env[%d]: %s\n", i, envc[i]);
        i++;
    }
}

int main(int argc, char **argv, char **envp)
{ 
    char **copy = ms_env_gen(envp);
    if (!copy)
    {
        fprintf(stderr, "Kopieren des Environments fehlgeschlagen.\n");
        return 1;
    }

    printf("Kopiertes Environment:\n");
    ms_print_env(copy);

    ms_free_envc(copy);
    return 0;
}