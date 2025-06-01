/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:58:45 by reriebsc          #+#    #+#             */
/*   Updated: 2025/05/29 13:48:28 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/enviroment.h"
#include <stdio.h>
#include <string.h>

//size_t	ft_strlen(const char *str)
//{
//	size_t	len;
//
//	len = 0;
//	while (str[len])
//		len++;
//	return (len);
//}
//
//char	*ft_strdup(const char *s1)
//{
//	char	*str;
//	int		i;
//
//	i = 0;
//	str = (char *)malloc(sizeof (const char) * ft_strlen(s1) + 1);
//	if (str)
//		str[ft_strlen(s1)] = '\0';
//	else
//		return (NULL);
//	while (s1[i])
//	{
//		str[i] = s1[i];
//		i++;
//	}
//	return (str);
//}

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

char    **ms_env_gen(char **env)
{
    int x;
    char    **envc;

    x = 0;
    while (env[x])
        x++;
    envc = malloc(x + 1 * sizeof(char *));
    if (!envc)
        return (NULL);
    envc[x] = NULL;
    x = 0;
    while (env[x])
    {
        envc[x] = ft_strdup(env[x]);
        if (!envc[x])
            return (ms_free_envc(envc), NULL);
        ++x;
    }
    return (envc);    
}

//void ms_print_env(char **envc)
//{
//    int i = 0;
//    while (envc[i])
//    {
//        printf("Kopiertes Environment:\n");
//        printf("env[%d]: %s\n", i, envc[i]);
//        i++;
//    }
//}
//
//int main(int argc, char **argv, char **env)
//{ 
//    char **copy = ms_env_gen(env);
//    if (!copy)
//    {
//        fprintf(stderr, "Kopieren des Environments fehlgeschlagen.\n");
//        return 1;
//    }
//
//    printf("Kopiertes Environment:\n");
//    ms_print_env(copy);
//
//    ms_free_envc(copy);
//    return 0;
//}