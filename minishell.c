/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reriebsc <reriebsc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:59:39 by reriebsc          #+#    #+#             */
/*   Updated: 2025/06/22 14:22:03 by reriebsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

static void	handle_shell_input(char *line)
{
	t_ms_command	*command;
	t_ms_sequence	*sequence;

	command = NULL;
	if(is_empty_str(line))
	{
		gc_free_ptr(line);
		return ;
	}
	add_history(line);
	sequence = parse(line);
	if (sequence == NULL)
	{
		if (ms_minishell_get()->exit_status == 0)
			ms_minishell_get()->exit_status = 1;
		return (gc_free_ptr(line));
	}
	if (!traverse_heredocs(sequence))
	{
		free_ast_node(sequence);
		gc_free_ptr(line);
		if (ms_minishell_get()->exit_status == EXIT_SUCCESS)
			ms_minishell_get()->exit_status = EXIT_FAILURE;
		return ;
	}
	return (tree_monitor(sequence, command), free_ms__sequence(), gc_free_ptr(line));
}


//This fuction will be called if the main function is startet with some Arguments and building a string out of the arguments 
void	non_interactive_arg(char **args, int argc)
{
	char	*line;

	line = join_str_array(args, argc);
	//add_history(line);
	handle_shell_input(line);
}

// Setzt Signal-Handler für z.B. Ctrl-C
// Schleife läuft solange kein exit()
// Prompt holen
// Speicherverwaltung (garbage collector)
// Zeile parsen und ausführen
void	interactive(void)
{
	char	*user_prompt;

	user_prompt = NULL;
	main_signals();
	while (!ms_minishell_get()->exit_status)
	{
		if (!get_user_prompt_value(&user_prompt))
			break ;
		gc_add(user_prompt);
		handle_shell_input(user_prompt);
	}
}

//Wird aufgerufen, wenn Minishell nicht interaktiv läuft (z. B. bei Pipe oder Datei-Umleitung).
//Startet eine Schleife, solange exit_status der Shell nicht gesetzt ist.
//Liest eine Zeile von stdin mit readline(NULL) (ohne Prompt).
//Übergibt die Zeile an den Garbage Collector mit gc_add() zur Speicherverwaltung.
//Wenn readline() NULL zurückgibt (z. B. bei EOF), wird die Schleife beendet.
//Leere Zeilen ("") werden ignoriert und Speicher wird freigegeben.
//Jede gültige Eingabezeile wird an handle_shell_input(line) übergeben:
void	minishell_non_interactive(void)
{
	char	*line;

	while (!ms_minishell_get()->exit_status)
	{
		line = gc_add(readline(NULL));
		if (line == NULL)
			break ;
		if (*line == '\0')
		{
			gc_free_ptr(line);
			continue ;
		}
		//add_history(line);
		handle_shell_input(line);
	}
}