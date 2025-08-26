/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:00:00 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/26 17:29:46 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_unused_fds(void)
{
	int	i;

	i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}

void	restore_std_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

void	restore_and_clear_std_fds(t_shell *shell, int stdin, int stdout)
{
	restore_std_fds(stdin, stdout);
	shell->execution->saved_stdin = -1;
	shell->execution->saved_stdout = -1;
}

static int	save_fds_and_check(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		if (*saved_stdin != -1)
			close(*saved_stdin);
		if (*saved_stdout != -1)
			close(*saved_stdout);
		return (1);
	}
	return (0);
}

int	save_and_redirect(t_redir *redirs, t_shell *shell, int *saved_stdin,
		int *saved_stdout)
{
	int	result;

	*saved_stdin = -1;
	*saved_stdout = -1;
	if (!redirs)
		return (0);
	if (save_fds_and_check(saved_stdin, saved_stdout) != 0)
		return (1);
	shell->execution->saved_stdin = *saved_stdin;
	shell->execution->saved_stdout = *saved_stdout;
	result = handle_redirections(redirs, shell);
	if (result != 0)
	{
		restore_std_fds(*saved_stdin, *saved_stdout);
		shell->execution->saved_stdin = -1;
		shell->execution->saved_stdout = -1;
		*saved_stdin = -1;
		*saved_stdout = -1;
		return (result);
	}
	return (0);
}
