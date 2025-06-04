/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:58:45 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/04 12:24:13 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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

//void	ms_free_envc(char **envc)
//{
//	int	x;
//
//	x = 0;
//	if (!envc)
//		return ;
//	while (envc[x] != NULL)
//	{
//		free(envc[x]);
//		++x;
//	}
//	free(envc);
//}
//
//char    **ms_env_gen(char **env)
//{
//    int x;
//    char    **envc;
//
//    x = 0;
//    while (env[x])
//        x++;
//    envc = malloc(x + 1 * sizeof(char *));
//    if (!envc)
//        return (NULL);
//    envc[x] = NULL;
//    x = 0;
//    while (env[x])
//    {
//        envc[x] = ft_strdup(env[x]);
//        if (!envc[x])
//            return (ms_free_envc(envc), NULL);
//        ++x;
//    }
//    return (envc);    
//}

void    ms_split_key_value(const char *str, char *key, char *value)
{
    int x;
    int y;

    x = 0;
    y = 0;
    while (str[x] != '=')
    {
        key[x] = str[x];
        ++x;
    }
    key[x] = '\0';
    x +=1;
    while (str[x])
    {
        value[y] = str[x];
        ++x;
        ++y; 
    }
    value[y] = '\0';
}

t_minishell *ms_minishell_get(void)
{
    static  t_minishell minishell;

    return (&minishell);
}

bool	ms_add_env_node(const char *key, const char *value)
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
	return (ft_lstadd_back(&ms_minishell_get()->env, new_node), true);
}
//**************************************************** */
// Wir bekommen einen kompletten Path des Enviroments mit Schlüssel und Value und plitten diesen auf
//
bool ms_add_env_dict(const char *str)
{
    char    *key;
    char    *value;

    ms_split_key_value(*str, key, value);
    return (ms_add_env_node(key, value));
}

t_dict_env  *ms_get_env_node(const char *key)
{
    t_list  *gen_node;

    gen_node = ms_minishell_get()->env;
    while (gen_node)
    {
        if (ft_strncmp(((t_dict_env *)gen_node->content)->key, key, ft_strlen(key)) == 0)
        {
            return ((t_dict_env *)gen_node->content);
        }
        gen_node = gen_node->next;
    }
    return (NULL);
}

void    ms_open_shells(void)
{
    t_dict_env  *node;
    char        *str;
    int         numb_shells;

    node = ms_get_env_node("SHLVL");
    if (!node)
    {
        ms_add_env_dict("SHLVL=1");
        return ;
    }
    numb_shells = ft_atoi(node->value);
    numb_shells += 1;
    str = ft_itoa(numb_shells);
    free(node->value);
    node->value = str;
}

bool    ms_set_env_value(const char key, const char value)
{
    t_dict_env  *node;
    if (!key)
        return (false);
    node = ms_get_env_node(&key);
    if (!value)
    {
        if (node && node->value)
            return (true);
        return (ms_add_env_node(&key, NULL));
    }
    if (node)
    {
        free(node->value);
        node->value = ft_strdup(&value);
        return (true);
    }
    return (ms_add_env_node(&key, &value));
}


//**************************************************** */
// Wir generieren eine Liste die da Grundelgende Enviroment der Shell Copiert und Rebuildet
//
bool    ms_generate_env(char **env, t_list *env_dict)
{
    int x;
    char    *cwd;

    x = 0;
    while (env[x])
    {
        if (!ms_add_env_dict(env[x]))
            return (false);
        ++x;
    }
    ms_open_shells();
    ms_set_env_val("OLDPWD", NULL);
    if (getcwd(cwd, 250))
        ms_set_env_val("PWD", cwd);
    return (true);
}




void ms_print_env(char **envc)
{
    int i = 0;
    while (envc[i])
    {
        printf("Kopiertes Environment:\n");
        printf("env[%d]: %s\n", i, envc[i]);
        i++;
    }
}

int main(int argc, char **argv, char **env)
{ 
    t_minishell *minishell;

    ms_env_gen(env, minishell->env);
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