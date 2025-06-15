/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 09:30:02 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/15 12:51:40 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "token.h"

t_ms_token	*ms_insert_token(t_ms_token *token, int8_t index)
{
	t_ms_token *const	new_token = malloc(sizeof(t_ms_token));

	if (new_token == NULL)
		return (NULL);
	new_token -> next = NULL;
	new_token -> index = index;
	if (token == NULL)
		return (new_token);
	new_token -> next = token -> next;
	token -> next = new_token;
	return (new_token);
}

bool	ms_isspace(char character)
{
	return ((character >= 9 && character <= 13) || character == 32);
}
