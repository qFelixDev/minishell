/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:28:53 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/04 14:36:34 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "../libft_extend/libft.h"
#include <unistd.h>
#include <stdbool.h>


//*************************************************************/
// Structs 
//*************************************************************/

typedef struct s_dict_env
{
	char				*key;
	char				*value;
}						t_dict_env;

typedef struct s_minishell
{
    t_list          *env;
}                   t_minishell;


//*************************************************************/
// Enviroment
//*************************************************************/
void        ms_split_key_value(const char *str, char *key, char *value);
t_minishell *ms_minishell_get(void);
int		    ms_add_env_node(const char *key, const char *value);
int	        ms_add_env_dict(const char *str);
t_dict_env  *ms_get_env_node(const char *key);
void        ms_open_shells(void);
bool        ms_set_env_value(const char *key, const char *value);
int	        ms_generate_env(char **env);

#endif