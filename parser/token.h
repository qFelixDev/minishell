/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:50:26 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/15 12:22:55 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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

typedef struct s_ms_token
{
	struct s_ms_token	*next;
	int8_t				index;
	char				*content;
	bool				concatenate_content;
}	t_ms_token;

typedef struct s_ms_sequence
{
	size_t	object_count;
	int8_t	operator;
	void	**objects;
	uint8_t	*is_sequence;
}	t_ms_sequence;

typedef struct s_ms_command
{
	char	**argv; // [0] = echo, [1] = "string", [2] = "noch ein string", [3] = "reriebsc"
	char	***redirects;

	char	**input_redirect;
	char	**output_redirect;
	char	**delim_redirect;
	char	**append_redirect;
}	t_ms_command;

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