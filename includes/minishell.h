/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:28:53 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/22 13:24:03 by reriebsc         ###   ########.fr       */
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
// ERROt Message Makros
//*************************************************************/

# ifndef DEBUG
#  define DEBUG 0
# endif

# ifndef BANNER
#  define BANNER ""
# endif

# define MAX_VAR_LEN 131072

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define RESET "\033[0m"

# define MAX_PATH 1024

# define ERROR_GET_LINE "ERROR WITH GETTING THE LINE"
# define ERROR_SPLIT "ERROR WITH SPLIT FUNCTION"
# define ERROR_INITIALISE "ERROR WITH INITIALISING COMMAND"
# define ERROR_NODE "ERROR CREATING THE NODE"
# define ERROR_MALLOC "ERROR ALLOCATION MEMORY"
# define ERROR_FIND_ENV "ERROR FINDING PATH ENV"
# define ERROR_EXECVE "ERROR EXECVE FAILED (COMMAND NOT FOUND)"
# define ERROR_HEREDOC "ERROR OPENING TEMPORARY HEREDOC"
# define ERROR_HEREDOC_SIGNAL "EXITING HEREDOC"
# define ERROR_TOO_ARGUMENT "ERROR TOO MUCH ARGUMENT"
# define ERROR_DIGIT_ARGUMENT "ERROR ARGUMENT IS NOT DIGIT"
# define ERROR_SET_ARGUMENT "ERROR : COMMAND SHOULD BE ./minishell 'commande'"
# define ERROR_PATH "ERROR WITH GETTING PATH"
# define ERROR_FORK "ERROR FORK FAILED"
# define ERROR_OPEN_FILE "ERROR OPEN FILE"

// real command
# define ERROR_FOUND_COMMAND "zsh: command not found."
# define ERROR_PWD "pwd : too many arguments"
# define ERROR_EXPORT "inconsistant type for assignement"







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
	int				exit_status;
	int				ready_exit;
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
// Signals 
//*************************************************************/
char	*create_prompt(void);
int		get_user_prompt_value(char **value);


//*************************************************************/
// Signals 
//*************************************************************/
void	sighandler(int sig);
void	main_signals(void);
void	reset_signals(void);


//*************************************************************/
// UTILS
//*************************************************************/
char	*join_str_array(char **list, int size);

//*************************************************************/
// Garbage Collector
//*************************************************************/
void	gc_list_clear(t_list **list, void (*del)(void *));
void	gc_free_ptr(void *addr);
void	gc_free(void);
void	gc_close_fds(void);
void	gc_close_fd(int fd);
int		gc_add_fd(int fd);


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
char		**ms_gen_env(void);
void		free_env_entry(void *content);


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


//*************************************************************/
// Memory Management
//*************************************************************/

void	gc_free(void);
void	*gc_malloc(size_t size);
void	*gc_add(void *ptr);
void	gc_free_ptr(void *addr);
void	gc_list_clear(t_list **list, void (*del)(void *));
void	gc_close_fd(int fd);
int		gc_add_fd(int fd);
void	gc_close_fds(void);

#endif