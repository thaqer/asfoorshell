/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 00:52:33 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/23 16:55:17 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_ast(t_ast *ast, t_shell *shell)
{
	if (!ast)
		return (0);
	if (ast->type == AST_SIMPLE_COMMAND)
		return (execute_simple_command(ast, shell));
	else if (ast->type == AST_PIPELINE)
		return (execute_pipeline(ast, shell));
	else if (ast->type == AST_LOGICAL_AND)
		return (execute_logical_and(ast, shell));
	else if (ast->type == AST_LOGICAL_OR)
		return (execute_logical_or(ast, shell));
	else if (ast->type == AST_SUBSHELL)
		return (execute_subshell(ast, shell));
	return (1);
}

static int	execute_subshell_perent(pid_t pid)
{
	int		status;

	setup_signals(SIGNAL_NONE);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_subshell(t_ast *ast, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		setup_signals(SIGNAL_CHILD);
		if (ast->s_subshell.redirs)
			handle_redirections(ast->s_subshell.redirs, shell);
		if (ast->s_subshell.child)
			execute_ast(ast->s_subshell.child, shell);
		cleanup(shell);
		exit(shell->status);
	}
	else
		return (execute_subshell_perent(pid));
	return (0);
}

int	execute_simple_command(t_ast *ast, t_shell *shell)
{
	char	**argv;
	int		result;
	int		saved_stdin;
	int		saved_stdout;

	argv = ast->s_simple_command.argv;
	result = save_and_redirect(ast->s_simple_command.redirs, shell,
			&saved_stdin, &saved_stdout);
	if (result != 0)
	{
		restore_and_clear_std_fds(shell, saved_stdin, saved_stdout);
		return (result);
	}
	if (!argv || !argv[0])
	{
		restore_and_clear_std_fds(shell, saved_stdin, saved_stdout);
		return (0);
	}
	if (is_builtin_command(argv[0]))
		result = execute_builtin(argv, shell);
	else
		result = execute_external_command(argv, shell);
	restore_and_clear_std_fds(shell, saved_stdin, saved_stdout);
	return (result);
}

void	preprocess_left_heredocs(t_ast *ast, t_shell *shell)
{
	t_redir	*current;

	if (!ast)
		return ;
	if (ast->type == AST_SIMPLE_COMMAND && ast->s_simple_command.redirs)
	{
		current = ast->s_simple_command.redirs;
		while (current)
		{
			if (current->type == REDIR_HEREDOC)
			{
				fprintf(stderr, "Executing pipeline...%s\n", current->target);
				current->heredoc_fd = handle_heredoc_redir(current->target,
						shell);
			}
			current = current->next;
		}
	}
}
