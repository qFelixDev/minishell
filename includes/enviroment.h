/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 10:49:15 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/01 14:27:06 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIROMENT_H
# define ENVIROMENT_H

#include <stdlib.h>
#include "../libft_extend/libft.h"

char    **ms_env_gen(char **envp);
void	ms_free_envc(char **envc);

#endif