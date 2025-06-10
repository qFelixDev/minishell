/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:50:26 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/10 15:27:22 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	MS_TOKEN_SHADOW_STRING
}

size_t		ms_count_index(t_ms_token *token, int8_t index);
const char	*ms_get_identity(int8_t index);
void		ms_free_tokens(t_ms_token *token, bool detach_only);
int8_t		ms_next_operator(t_ms_token *token);