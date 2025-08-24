/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_operators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:00:00 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/16 00:00:00 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_left_pipeline(int *pipefd, t_ast *ast, t_shell *shell)
{
	int	ret;

	setup_signals(SIGNAL_CHILD);
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	ret = execute_ast(ast->s_binary.left, shell);
	cleanup(shell);
	exit(ret);
}

void	handle_right_pipeline(int *pipefd, t_ast *ast, t_shell *shell)
{
	int	ret;

	setup_signals(SIGNAL_CHILD);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	ret = execute_ast(ast->s_binary.right, shell);
	cleanup(shell);
	exit(ret);
}

int	execute_logical_and(t_ast *ast, t_shell *shell)
{
	int	left_status;

	left_status = execute_ast(ast->s_binary.left, shell);
	if (left_status != 0)
		return (left_status);
	return (execute_ast(ast->s_binary.right, shell));
}

int	execute_logical_or(t_ast *ast, t_shell *shell)
{
	int	left_status;

	left_status = execute_ast(ast->s_binary.left, shell);
	if (left_status == 0)
		return (left_status);
	return (execute_ast(ast->s_binary.right, shell));
}
