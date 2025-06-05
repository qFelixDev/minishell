/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:12:20 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/05 09:59:02 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "libft_extend/libft.h"

enum
{
	MS_TOKEN_AND,
	MS_TOKEN_OR,
	MS_TOKEN_PIPE,
	MS_TOKEN_DELIM,
	MS_TOKEN_APPEND,
	MS_TOKEN_INPUT,
	MS_TOKEN_OUTPUT,
	MS_TOKEN_OPEN,
	MS_TOKEN_CLOSE,
	MS_TOKEN_STRING_OPAQUE,
	MS_TOKEN_STRING_TRANSLUCENT,
	MS_TOKEN_STRING_TRANSPARENT,
	MS_TOKEN_MAX
};

enum
{
	MS_TOKEN_NONE = -1,
	MS_TOKEN_AND,
	MS_TOKEN_OR,
	MS_TOKEN_PIPE,
	MS_TOKEN_BRACKET_OPEN,
	MS_TOKEN_BRACKET_CLOSE,
	MS_TOKEN_HEREDOC,
	MS_TOKEN_APPEND,
	MS_TOKEN_INPUT,
	MS_TOKEN_OUTPUT,
	MS_TOKEN_WILDCARD,
	MS_TOKEN_STRING,
	MS_TOKEN_VARIABLE
}

typedef struct s_ms_token
{
	struct s_ms_token	*next;
	int8_t				index;
	char				*content;
	bool				concatenate_content;
}	t_ms_token;

typedef struct s_ms_command
{
	char	**argv;
	char	*redirect_delim;
	char	*redirect_append;
	char	*redirect_input;
	char	*redirect_output;
}	t_ms_command;

typedef struct s_ms_sequence
{
	void	**objects;
	uint8_t	*is_sequence;
	size_t	object_count;
}	t_ms_sequence;

int8_t	ms_next_operator(t_ms_token *token);
void	ms_free_tokens(t_ms_token *token);
bool	ms_insert_token(t_ms_token *predecessor, int8_t index);
bool	ms_contains_index(t_ms_token *token, int8_t index);