/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:03:14 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/26 14:59:14 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <signal.h>

typedef struct s_shell			t_shell;
typedef struct s_ast			t_ast;
typedef struct s_redir			t_redir;

//remove this line when we intgrate with the main project

typedef enum e_signal_mode
{
	SIGNAL_PARENT,
	SIGNAL_CHILD,
	SIGNAL_HEREDOC,
	SIGNAL_NONE
}								t_signal_mode;

extern volatile sig_atomic_t	g_sig_value;

typedef struct s_execution
{
	int							saved_stdin;
	int							saved_stdout;
}								t_execution;

int								execute_ast(t_ast *ast, t_shell *shell);
int								execute_simple_command(t_ast *ast,
									t_shell *shell);
int								execute_pipeline(t_ast *ast, t_shell *shell);
int								execute_logical_and(t_ast *ast, t_shell *shell);
int								execute_logical_or(t_ast *ast, t_shell *shell);
int								execute_subshell(t_ast *ast, t_shell *shell);

// Helper functions
int								execute_external_command(char **argv,
									t_shell *shell);
int								handle_redirections(t_redir *redirs,
									t_shell *shell);
char							*find_command_in_path(char *cmd,
									t_shell *shell);

// Utility functions
char							*search_in_path_dirs(char **paths, char *cmd);
int								fork_and_execute(char *command_path,
									char **argv,
									t_shell *shell);
int								save_and_redirect(t_redir *redirs,
									t_shell *shell,
									int *saved_stdin,
									int *saved_stdout);
void							restore_std_fds(int saved_stdin,
									int saved_stdout);
void							restore_and_clear_std_fds(t_shell *shell,
									int saved_stdin,
									int saved_stdout);
void							handle_left_pipeline(int *pipefd, t_ast *ast,
									t_shell *shell);
void							handle_right_pipeline(int *pipefd, t_ast *ast,
									t_shell *shell);
void							preprocess_left_heredocs(t_ast *ast,
									t_shell *shell);
void							preprocess_all_heredocs(t_ast *ast,
									t_shell *shell);
void							cleanup_heredoc_fds(t_ast *ast);
void							close_unused_fds(void);

// signals
void							setup_signals(int mode);
void							handle_signal(int sig);
void							handle_signal_heredoc(int sig);
void							handle_signal_child(int sig);
void							handle_first(int sig);
void							handle_sigpipe(int sig);

#endif
