/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:28:53 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/08 10:20:25 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include "../libft_extend/libft.h"
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>




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

typedef enum e_token_type
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
	MS_TOKEN_MAX,
	MS_TOKEN_REDIRECT_INPUT,
	MS_TOKEN_REDIRECT_OUTPUT,
	MS_TOKEN_REDIRECT_APPEND,
	MS_TOKEN_REDIRECT_INPUT_APPEND,
}						t_token_type;

//*************************************************************/
// STRUCTS
//*************************************************************/

///////////////Redirect Struct
////////////////////////////////////////////////////////////////

typedef struct s_redirect
{
	int					fd_left;
	int					fd_right;
	char				*file;
	t_token_type		type;
}						t_redirect;

///////////////Pipe Struct
////////////////////////////////////////////////////////////////

typedef struct s_pipe_data
{
	int					pipe_fds[2];
	pid_t				left_pid;
	pid_t				right_pid;
	int					right_status;
	int					right_result;
}						t_pipe_data;

///////////////SHELL STRUCT
////////////////////////////////////////////////////////////////

typedef struct s_minishell
{
	t_list			*env;
	int				exit_status;
	int				ready_exit;
	bool			or_sequenze;
	bool			finish_or;
	int				heredoc_failed;
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
	char	***redirects; // [redirect][index]
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

///////////////Escape Date 
////////////////////////////////////////////////////////////////

typedef struct s_escape_data
{
	char				*str;
	int					*i;
	int					*j;
	bool				*in_quotes;
	char				*quote_type;
	int					len;
}						t_escape_data;

//*************************************************************/
// Redirections
//*************************************************************/

size_t			*get_char_count(void);
char			*strlst_to_str(t_list *result_chars);
bool			add_str_to_result(const char *str, size_t *i,
					t_list **result_chars, size_t len);
char			*filter_and_get_redirects(t_ms_command *input,
					t_list **redirects, int *exit_code);
bool			traverse_heredocs(t_ms_sequence *sequence);
bool			parse_redirect(t_list **redirects, const char *str,
					t_token_type redirect_type, size_t *i);
bool			is_escaped(const char *str, size_t i);
bool			skip_quotes(const char *str, size_t *i);
char			**split_quotes(char const *str);
size_t			count_words(char const *str);
char			*ft_unescape_string(char *str);
bool			process_heredoc(t_list *redirects, t_ms_sequence *sequence);
void			redirect_input_from_heredoc(const char *filename);
void			convert_pointer_to_string(char *str, void *ptr);
bool			redirection_heredoc(const char *delimiter,
					t_ms_sequence *sequence);

//*************************************************************/
// Pipes
//*************************************************************/
void			pipe_left_process(t_ms_sequence *sequence,
					t_pipe_data *pipe_data);
void			pipe_right_process(t_ms_sequence *sequence,
					t_pipe_data *pipe_data);
void			pipe_fork_error(t_pipe_data *pipe_data);
void			close_pipe_and_wait(t_pipe_data *pipe_data);
void			pipe_monitor(t_ms_sequence *sequence);

//*************************************************************/
// Commands
//*************************************************************/
int				get_user_prompt_value(char **value);
char			*create_prompt(void);

//*************************************************************/
// Shell
//*************************************************************/
int				ms_exit(int state);
void			non_interactive_arg(char **args, int argc);
void			interactive(void);
void			minishell_non_interactive(void);
int				tree_monitor(t_ms_sequence *sequence);


//*************************************************************/
// Signals 
//*************************************************************/
char			*create_prompt(void);
int				get_user_prompt_value(char **value);


//*************************************************************/
// Signals 
//*************************************************************/
void			sighandler(int sig);
void			main_signals(void);
void			reset_signals(void);
void			command_signals(void);


//*************************************************************/
// UTILS
//*************************************************************/
char			*join_str_array(char **list, int size);
void			ft_free_cluster(char	**cluster);

//*************************************************************/
// Garbage Collector
//*************************************************************/
void			gc_list_clear(t_list **list, void (*del)(void *));
void			gc_free_ptr(void *addr);
void			gc_free(void);
void			gc_close_fds(void);
void			gc_close_fd(int fd);
int				gc_add_fd(int fd);
void			free_redirect(void *content);


//*************************************************************/
// ENVIROMENT
//*************************************************************/
void			ms_split_key_value(const char *str, char *key, char *value);
t_minishell		*ms_minishell_get(void);
int				ms_add_env_node(const char *key, const char *value);
int				ms_add_env_dict(const char *str);
t_dict_env		*ms_get_env_node(const char *key);
void			ms_open_shells(void);
bool			ms_set_env_value(const char *key, const char *value);
int				ms_generate_env(char **env);
char			*ft_getenv(char *name);
char			**ms_gen_env(void);
void			free_env_entry(void *content);


//*************************************************************/
// BUILTINS
//*************************************************************/
int				ms_pwd(void);
int				ms_cd(char *path);
int				ms_print_env(void);
int				ms_unset(t_ms_command *command);
int				ms_echo(t_ms_command *command, bool is_n);
int				ms_export(t_ms_command *command);

//*************************************************************/
// PARSING
//*************************************************************/

t_ms_sequence	*ms_parse(char *string);
t_ms_command	*ms_get_command(t_ms_token *token);
void			ms_free_sequence(t_ms_sequence *sequence);
void			ms_free_command(t_ms_command *command);
void			ms_print_sequence(t_ms_sequence *sequence);
void			ms_print_command(t_ms_command *command);



//*************************************************************/
// Memory Management
//*************************************************************/

void			gc_free(void);
void			*gc_malloc(size_t size);
void			*gc_add(void *ptr);
void			gc_free_ptr(void *addr);
void			gc_list_clear(t_list **list, void (*del)(void *));
void			gc_close_fd(int fd);
int				gc_add_fd(int fd);
void			gc_close_fds(void);

#endif