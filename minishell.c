/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghodges <ghodges@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:59:39 by reriebsc          #+#    #+#             */
/*   Updated: 2025/07/19 14:46:11 by ghodges          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	tree_monitor(t_ms_sequence *sequence);

// (1) Ignoriere komplett leere Strings (nur Whitespace)
// (2) Eingabe in die History (readline-History)
// (3) Parsen der Eingabe → liefert Sequenzbaum (AST)
// (4) Fehler beim Parsen → Exit-Code setzen
// (5) Heredocs verarbeiten (z. B. "<< EOF")
// (6) Ausführen der AST-Befehle → Tree-Traversierung
// Aufräumen der AST-Struktur
// Prompt-Zeile freigeben
static void	handle_shell_input(char *line)
{
	t_ms_sequence	*sequence;

	if (line[0] == '\0')
		return ;
	add_history(line);
	sequence = ms_parse(line);
	if (sequence == NULL)
	{
		if (ms_minishell_get()->exit_status == 0)
			ms_minishell_get()->exit_status = 258;
		return (gc_free_ptr(line));
	}
	//if (!traverse_heredocs(sequence))
	//{
	//	ms_free_sequence(sequence);
	//	gc_free_ptr(line);
	//	if (ms_minishell_get()->exit_status == EXIT_SUCCESS)
	//		ms_minishell_get()->exit_status = EXIT_FAILURE;
	//	return ;
	//}
	//ms_print_sequence(sequence, 0);
	return (ms_execute_sequence(sequence), ms_free_sequence(sequence),
		gc_free_ptr(line));
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
	int		tty;

	tty = gc_add_fd(open("/dev/tty", O_WRONLY));
	user_prompt = NULL;
	main_signals();
	while (get_user_prompt_value(&user_prompt, tty))
	{
		gc_add(user_prompt);
		//struct timeval start, end;
		//gettimeofday(&start, NULL);
		handle_shell_input(user_prompt);
		//gettimeofday(&end, NULL);
		//printf("%ld\n", end.tv_usec - start.tv_usec + 1000000 * (end.tv_sec - start.tv_sec));
	}
	gc_close_fd(tty);
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
		handle_shell_input(line);
	}
}
