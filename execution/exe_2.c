/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 13:30:02 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/05 16:28:57 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//if (sequence->is_sequence[object_index / 8] & (1u << (object_index % 8)))
// if (sequence->is_sequence[i / 8] & (1u << (i % 8)))

static char	*ft_find_path(char *cmd)
{
	int		i;
	char	**path_all;
	char	*sub_path;
	char	*exe_path;

	if (ft_strchr(cmd, '/') != NULL)
		return (ft_strdup(cmd));
	i = 0;
	path_all = ft_split(ft_getenv("PATH"), ':');
	while (path_all[i])
	{
		sub_path = ft_strjoin(path_all[i], "/");
		exe_path = ft_strjoin(sub_path, cmd);
		free(sub_path);
		if (access(exe_path, F_OK | X_OK) == 0)
		{
			ft_free_cluster(path_all);
			return (exe_path);
		}
		free(exe_path);
		i++;
	}
	return (ft_free_cluster(path_all), NULL);
}

static char	*ft_find_exec_path(char **cmd_s, char **env)
{
	char	*path;

	if (!env || !*env)
	{
		path = ft_strjoin("/bin/", cmd_s[0]);
		if (access(path, X_OK) == -1)
		{
			free(path);
			path = ft_strjoin("/usr/bin/", cmd_s[0]);
		}
	}
	else
		path = ft_find_path(cmd_s[0]);
	return (path);
}

static void	handle_child_process(t_ms_command *command, char **env_cpy)
{
	char *path;

	path = ft_find_exec_path(command->argv, env_cpy);
	command_signals();
	execve(path, command->argv, env_cpy);
	//command_not_found(path);
	if (ms_minishell_get()->or_sequenze)
		ms_minishell_get()->finish_or = false;
	if (!ms_minishell_get()->or_sequenze)
		ms_exit(EXIT_FAILURE);
}

static int	wait_for_process(pid_t pid, char **env_cpy)
{
	int	status;
	int	sig_num;

	waitpid(pid, &status, 0);
	ft_free_cluster(env_cpy);
	main_signals();
	if (WIFSIGNALED(status))
	{
		printf("\n");
		sig_num = WTERMSIG(status);
		return (128 + sig_num);
	}
	return (WEXITSTATUS(status));
}

int	ms_execution_command(t_ms_command *command)
{
	pid_t	pid;
	char	**env_cpy;

	env_cpy = ms_gen_env();
	if (!env_cpy)
		return (perror(ERROR_MALLOC), 1);
	pid = fork();
	if (pid == -1)
		return (perror(ERROR_FORK), 1);
	if (pid == 0)
		handle_child_process(command, env_cpy);
	return (wait_for_process(pid, env_cpy));
}

//void	ms_and_structure(t_ms_sequence *sequence)
//{
//	int	object_index;
//
//	object_index = 0;
//	while (object_index < sequence->object_count)
//	{
//		if (sequence->is_sequence[object_index / 8] & (1u << (object_index % 8)))
//		{
//			if (sequence->operator == MS_TOKEN_AND)
//				ms_and_structure(sequence->objects[object_index]);
//			else if (sequence->operator == MS_TOKEN_OR)
//				ms_or_structure(sequence->objects[object_index]);
//			if (sequence->operator == MS_TOKEN_PIPE)
//				ms_pipe_structure(sequence->objects[object_index]);
//		}
//		else
//		{
//			//ms_and_prozess()
//			ms_execution_command(ms_get_command(sequence->objects[object_index]));
//		}
//		object_index++;
//	}
//}

//void	ms_and_exe(t_ms_sequence *sequence)
//{
//	int	object_index;
//	pid_t	pid;
//
//	object_index = 0;
//	while (object_index < sequence->object_count)
//	{
//		if (sequence->is_sequence[object_index / 8] & (1u << (object_index % 8)))
//		{
//			if (sequence->operator == MS_TOKEN_AND)
//				ms_and_exe(sequence->objects[object_index]);
//			else if (sequence->operator == MS_TOKEN_OR)
//				ms_or_exe(sequence->objects[object_index]);
//			if (sequence->operator == MS_TOKEN_PIPE)
//				ms_pipe_exe(sequence->objects[object_index]);
//		}
//		else
//		{
//			//ms_and_prozess()
//			ms_and_exe_v2(ms_get_command(sequence->objects[object_index]));
//		}
//		object_index++;
//	}
//}
//
//void	ms_exe_start(t_ms_sequence *sequence)
//{
//	if (sequence->operator == MS_TOKEN_AND)
//		ms_and_struture(sequence);
//	else if (sequence->operator == MS_TOKEN_OR)
//		ms_or_structure();
//	if (sequence->operator == MS_TOKEN_PIPE)
//		ms_pipe_structure();
//}


//char	*ft_find_path(char *cmd, char *env)
//{
//	int		i;
//	char	**path_all;
//	char	*sub_path;
//	char	*exe_path;
//
//	i = 0;
//	path_all = ft_split(env, ':');
//	while (path_all[i])
//	{
//		sub_path = ft_strjoin(path_all[i], "/");
//		exe_path = ft_strjoin(sub_path, cmd);
//		free(sub_path);
//		if (access(exe_path, F_OK | X_OK) == 0)
//		{
//			ft_free_cluster(path_all);
//			return (exe_path);
//		}
//		free(exe_path);
//		i++;
//	}
//	return (ft_free_cluster(path_all), NULL);
//}
//
//static char	*ft_find_exec_path(char **cmd_s, char *env)
//{
//	char	*path;
//
//	if (!env)
//	{
//		path = ft_strjoin("/bin/", cmd_s[0]);
//		if (access(path, X_OK) == -1)
//		{
//			free(path);
//			path = ft_strjoin("/usr/bin/", cmd_s[0]);
//		}
//	}
//	else
//		path = ft_find_path(cmd_s[0], env);
//	return (path);
//}
//
//void	ms_and_execute(t_ms_command *command)
//{
//	char *env;
//	char **path;
//	
//	env = ft_getenv("PATH");
//	path = ft_find_exec_path(command->argv, env);
//	execve(path, command->argv, ms_gen_env());
//	perror(command->argv[0]);
//	ft_free_cluster(command->argv[0]);
//	return (free(path), exit(127));
//}

//void	ms_and_exe_v2(t_ms_command *command)
//{
//	//buildin?;
//	char *env;
//	char **path;
//	pid_t	pid;
//
//	pid = fork();
//	if (pid == -1)
//		perror("fork");
//	if (pid == 0)
//		ms_and_execute(command);
//	else 
//		waitpid(pid, NULL, 0);
//}

//char	*ft_getenv(char *name)
//{
//	char		*str;
//	t_list		*env_head;
//	t_dict_env	*env;
//
//	env_head = ms_minishell_get()->env;
//	while (env_head)
//	{
//		env = env_head->content;
//		if (ft_strcmp(name, env->key) == 0)
//			return (env->value);
//		env_head = env_head->next;
//	}
//	return ("/usr/bin");
//}
//
//void	ms_print_env(void)
//{
//	t_dict_env	*node;
//	t_list		*env;
//
//	env = ms_minishell_get()->env;
//	while (env)
//	{
//		node = env->content;
//		if (env->next && !node->value)
//		{
//			env = env->next;
//			continue ;
//		}
//		printf("%s", node->key);
//		printf("=%s\n", node->value);
//		env = env->next;
//	}
//}



//int	main(int ac, char **av, char **env)
//{
//	if (!ms_generate_env(env))
//		return (1);
//	ms_print_env();
//	ms_cd("../includes");
//	ms_print_env();
//	return (0);
//}