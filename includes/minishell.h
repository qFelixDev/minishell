/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:28:53 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/24 14:49:37 by reriebsc         ###   ########.fr       */
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
# include <sys/stat.h>

//*************************************************************/
// ERROR Message Macros
//*************************************************************/

# ifndef DEBUG
#  define DEBUG 0
# endif

# ifndef BANNER
#  define BANNER ""
# endif

# define MAX_VAR_LEN 131072

# define RED "\1\033[1;31m\2"
# define GREEN "\1\033[1;32m\2"
# define YELLOW "\1\033[1;33m\2"
# define BLUE "\1\033[1;34m\2"
# define PURPLE "\1\033[1;35m\2"
# define RESET "\1\033[0m\2"

# define MAX_PATH 1024

# define ERROR_MALLOC "ERROR ALLOCATION MEMORY"
# define ERROR_FORK "ERROR FORK FAILED"

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
	MS_TOKEN_MAX
}						t_token_type;

typedef enum e_redirect_type
{
	MS_REDIRECT_DELIM,
	MS_REDIRECT_APPEND,
	MS_REDIRECT_INPUT,
	MS_REDIRECT_OUTPUT,
	MS_REDIRECT_MAX
}	t_redirect_type;

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
}				t_minishell;

///////////////ENVIRONMENT DICTIONARY
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
	int		*delim_descriptors;
	uint8_t	*is_sequence;
}	t_ms_sequence;

typedef struct s_ms_command
{
	char	**argv;
	char	***redirects; // [redirect][index]
	int		delim_descriptor;
}	t_ms_command;

///////////////TOKENS
////////////////////////////////////////////////////////////////

typedef struct s_ms_token
{
	struct s_ms_token	*next;
	int8_t				index;
	char				*content;
	uint32_t			concatenation;
	int					fd;
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
// Pipes
//*************************************************************/
int				pipe_monitor(t_ms_sequence *sequence);

//*************************************************************/
// Commands
//*************************************************************/
int				get_user_prompt_value(char **value, int tty);
char			*create_prompt(void);

//*************************************************************/
// Shell
//*************************************************************/
void			ms_exit(int state);
void			non_interactive_arg(char **args, int argc);
void			interactive(void);
void			minishell_non_interactive(void);
int				ms_execute_sequence(t_ms_sequence *sequence);
int				wait_for_process(pid_t pid);
void			handle_child_process(t_ms_command *command, char **env_cpy);
int				exe_manager(t_ms_command *command, int delim_descriptor);
void			generatebuildins(char *buildins[7]);
int				buildin_exe(t_ms_command *command, int index_buildin);
int				ms_execution_command(t_ms_command *command);
char			*ft_find_path(char *cmd);
char			*ft_find_exec_path(char **cmd_s, char **env);
void			handle_child_process(t_ms_command *command, char **env_cpy);
int				wait_for_process(pid_t pid);
bool			backup_std_fds(int *stdin_backup, int *stdout_backup);
int				try_execute_builtin(t_ms_command *command, char **build_ins,
					int stdin_backup, int stdout_backup);
void			restore_std_fds(int stdin_backup, int stdout_backup);
bool			handle_intermediate_redirects(char **redirects,
					size_t *last_index);
int				get_target_fd(int type);
int				get_open_flags(int type);
bool			open_redirect_fd(char *file, int type, int *fd_out);
bool			dup_and_close(int descriptor, int target_fd);
bool			apply_redirects(char **redirects, int type);
size_t			ms_get_pattern(t_ms_token *token, char *pattern);
size_t			ms_enumerate_matches(char *pattern, char *path, char **matches);
int				ms_read_delims(t_ms_token *token, t_ms_token *last);
bool			matches_pattern(char *pattern, char *string);
size_t			get_word_length(char *word);

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
size_t			ms_deflate_string(char *string, char *deflated);

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
bool			is_valid_identifier(const char *identifier);

//*************************************************************/
// BUILTINS
//*************************************************************/

int				ms_exit_builtin(t_ms_command *command);
int				ms_pwd(void);
int				ms_cd(char *path);
int				ms_print_env(void);
int				ms_unset(t_ms_command *command);
int				ms_echo(t_ms_command *command, bool is_n);
int				ms_export(char **argv);
//int			ms_export(t_ms_command *command);

//*************************************************************/
// PARSING
//*************************************************************/

t_ms_sequence	*ms_parse(char *string);
t_ms_command	*ms_get_command(t_ms_token *token);
void			ms_free_sequence(t_ms_sequence *sequence);
void			ms_free_command(t_ms_command *command);
void			ms_print_sequence(t_ms_sequence *sequence, int indentation);
void			ms_print_command(t_ms_command *command);

// Token
//*************************************************************/

size_t			ms_count_index(t_ms_token *token, int8_t index);
const char		*ms_get_identity(int8_t index);
void			ms_free_tokens(t_ms_token *token, bool detach_only);
void			ms_print_tokens(t_ms_token *token);
int8_t			ms_next_operator(t_ms_token *token);
t_ms_token		*ms_insert_token(t_ms_token *token, int8_t index);
bool			ms_isspace(char character);
t_ms_command	*ms_get_command(t_ms_token *token);
void			ms_print_command(t_ms_command *command);
void			ms_free_command(t_ms_command *command);

//*************************************************************/
// Pipes
//*************************************************************/

void			close_all_pipes(t_ms_sequence *seq, int pipes[1024][2]);
int				init_pipes(t_ms_sequence *sequence, int pipes[1024][2]);
void			child_process(t_ms_sequence *seq, int pipes[1024][2], size_t i);
int				fork_children(t_ms_sequence *sequence, int pipes[1024][2]);
void			wait_for_children(t_ms_sequence *seq, int *status);
int				pipe_monitor(t_ms_sequence *sequence);

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
void			*gc_add_list_node(t_list *list);

#endif
