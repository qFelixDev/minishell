/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 10:51:10 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/02 15:03:02 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_heredoc_input(int temp_fd, const char *delimiter)
{
	char	*buffer;
	char	*tmp;

	while (1)
	{
		buffer = gc_add(readline("heredoc> "));
		if (buffer == NULL)
		{
			write(temp_fd, "\n", 1);
			break ;
		}
		if (ft_strncmp(buffer, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			gc_free_ptr(buffer);
			break ;
		}
		tmp = expand_string(buffer); //No Idea Gabriel?
		gc_free_ptr(buffer);
		buffer = tmp;
		write(temp_fd, buffer, ft_strlen(buffer));
		write(temp_fd, "\n", 1);
		gc_free_ptr(buffer);
	}
}

static void	create_heredoc_file(const char *delimiter, const char *filename)
{
	int	temp_fd;

	signal_heredoc();
	temp_fd = gc_add_fd(open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600));
	if (temp_fd == -1)
	{
		perror("Error creating heredoc file");
		destroy_minishell(1);
	}
	handle_heredoc_input(temp_fd, delimiter);
	gc_close_fd(temp_fd);
	destroy_minishell(0);
}

void	redirect_input_from_heredoc(const char *filename)
{
	int	temp_fd;

	temp_fd = gc_add_fd(open(filename, O_RDONLY));
	if (temp_fd == -1)
	{
		unlink(filename);
		perror("Error opening heredoc file for reading");
		destroy_minishell(EXIT_FAILURE);
	}
	dup2(temp_fd, STDIN_FILENO);
	gc_close_fd(temp_fd);
	unlink(filename);
}

static int	handle_child_status(int status)
{
	t_minishell	*shell;

	if (WEXITSTATUS(status) == 130)
	{
		shell = ms_minishell_get();
		shell->exit_status = 130;
		shell->heredoc_failed = 1;
		return (perror(("Heredoc process interrupted by SIGINT\n")), 1);
	}
	else if (WEXITSTATUS(status) != 0)
		perror("Heredoc process interrupted\n");
	return (0);
}

bool	redirection_heredoc(const char *delimiter, t_ms_sequence *sequence)
{
	pid_t	pid;
	int		status;
	char	*file_name;

	file_name = gc_malloc(50);
	convert_pointer_to_string(file_name, sequence);
	pid = fork();
	reset_signals();
	if (pid == -1)
	{
		printf("Fork failed\n");
		destroy_minishell(perror("Fork failed"), 1); //Errror handling 
	}
	if (pid == 0)
		create_heredoc_file(delimiter, file_name);
	waitpid(pid, &status, 0);
	node->heredoc_filename = file_name;
	reset_signals();
	if (handle_child_status(status))
		return (false);
	return (true);
}
