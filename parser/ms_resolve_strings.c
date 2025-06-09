/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_resolve_strings.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 12:24:58 by ghodges           #+#    #+#             */
/*   Updated: 2025/06/09 12:34:53 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ms_resolve_strings(t_ms_token *token)
{
	while (token != NULL)
	{
		while (token != NULL && token -> index != MS_TOKEN_UNRESOLVED_STRING)
			token = token -> next;
	}
}
