/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:28:53 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/01 15:51:12 by reriebsc         ###   ########.fr       */
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

#endif