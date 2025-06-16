/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:28:53 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/16 09:20:25 by ghodges          ###   ########.fr       */
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
#include <stdint.h>

//*************************************************************/
// ENUM
//*************************************************************/

enum
{
	MS_TOKEN_NONE = -1,
	MS_TOKEN_AND,
	MS_TOKEN_OR,
	MS_TOKEN_PIPE,
	MS_TOKEN_OPEN,
	MS_TOKEN_CLOSE,
	MS_TOKEN_DELIM,
	MS_TOKEN_APPEND,
	MS_TOKEN_INPUT,
	MS_TOKEN_OUTPUT,
	MS_TOKEN_WILDCARD,
	MS_TOKEN_VARIABLE,
	MS_TOKEN_UNRESOLVED_STRING,
	MS_TOKEN_STRING,
	MS_TOKEN_SHADOW_STRING,
	MS_TOKEN_MAX
};

//*************************************************************/
// STRUCTS
//*************************************************************/

///////////////SHELL STRUCT
////////////////////////////////////////////////////////////////

typedef struct s_minishell
{
	t_list			*env;
}				t_minishell;

///////////////ENVIROMENT DICTIONARY
////////////////////////////////////////////////////////////////
typedef struct s_dict_env
{
	char				*key;
	char				*value;
}						t_dict_env;

////////SEQUENZ / Command (Command are in Objekts from sequents)
////////////////////////////////////////////////////////////////

typedef struct s_ms_sequence
{
	size_t	object_count;
	int8_t	operator;
	void	**objects;
	uint8_t	*is_sequence;
}	t_ms_sequence;

typedef struct s_ms_command
{
	char	**argv;
	char	*redirect_delim;
	char	*redirect_append;
	char	*redirect_input;
	char	*redirect_output;
}	t_ms_command;

///////////////TOKENS
////////////////////////////////////////////////////////////////

typedef struct s_ms_token
{
	struct s_ms_token	*next;
	int8_t				index;
	char				*content;
	bool				concatenate_content;
}	t_ms_token;


//*************************************************************/
// ENVIROMENT
//*************************************************************/
void		ms_split_key_value(const char *str, char *key, char *value);
t_minishell	*ms_minishell_get(void);
int			ms_add_env_node(const char *key, const char *value);
int			ms_add_env_dict(const char *str);
t_dict_env	*ms_get_env_node(const char *key);
void		ms_open_shells(void);
bool		ms_set_env_value(const char *key, const char *value);
int			ms_generate_env(char **env);
char		*ft_getenv(char *name);


//*************************************************************/
// BUILTINS
//*************************************************************/
int			ms_pwd(void);
static int	ms_cd(char *path);
void		ms_print_env(void);


//*************************************************************/
// PARSING
//*************************************************************/

t_ms_sequence	*ms_parse(char* string);
t_ms_command	*ms_get_command(t_ms_token *token);
void			ms_free_sequence(t_ms_sequence *sequence);
void			ms_free_command(t_ms_command *command);
void			ms_print_sequence(t_ms_sequence *sequence);
void			ms_print_command(t_ms_command *command);

#endif