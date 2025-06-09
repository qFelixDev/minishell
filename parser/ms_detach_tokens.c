/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_detach_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 10:24:42 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/09 10:36:00 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ms_detach_tokens(t_ms_token *token)
{
	t_ms_token	*temp;

	while (token != NULL)
	{
		temp = token;
		token = token -> next;
		if (token -> index < MS_TOKEN_DELIM)
			temp -> next = NULL;
		if (temp -> index < MS_TOKEN_DELIM)
			free(temp);
	}
}
