/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 13:30:02 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/15 14:23:40 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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


//sif (sequence -> is_sequence[object_index / 8]
//s	& (1u << (object_index % 8)))


void	ms_exe_start(t_ms_sequence *sequence)
{
	size_t	i;

	i = 0;
	while (i <= sequence->object_count - 1)
	{
		if (sequence->is_sequence[i / 8] & (1u << (i % 8)))
			ms_exe_start(sequence);
		else
			ms_exe_command(sequence);
		++i;
	}
}

char	*ft_getenv(char *name)
{
	char		*str;
	t_list		*env_head;
	t_dict_env	*env;

	env_head = ms_minishell_get()->env;
	while (env_head)
	{
		env = env_head->content;
		if (ft_strcmp(name, env->key) == 0)
			return (env->value);
		env_head = env_head->next;
	}
	return ("/usr/bin");
}

void	ms_print_env(void)
{
	t_dict_env	*node;
	t_list		*env;

	env = ms_minishell_get()->env;
	while (env)
	{
		node = env->content;
		if (env->next && !node->value)
		{
			env = env->next;
			continue ;
		}
		printf("%s", node->key);
		printf("=%s\n", node->value);
		env = env->next;
	}
}

int	main(int ac, char **av, char **env)
{
	if (!ms_generate_env(env))
		return (1);
	ms_print_env();
	ms_cd("../includes");
	ms_print_env();
	return (0);
}