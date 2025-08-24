/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 01:24:07 by daniah            #+#    #+#             */
/*   Updated: 2025/08/25 01:50:23 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//try to ctr d after << 1 + ctr c and (echo $?)
static void	preprocess_heredoc_redirs(t_redir *redirs, t_shell *shell)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			fprintf(stderr, "Executing pipeline...%s\n", current->target);
			current->heredoc_fd = handle_heredoc_redir(current->target, shell);
			if (current->heredoc_fd == -2)
				break ;
		}
		current = current->next;
	}
}

static void	preprocess_subshell_heredocs(t_ast *ast, t_shell *shell)
{
	t_redir	*current;

	current = ast->s_subshell.redirs;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			fprintf(stderr, "ssssssssExecuting pipeline...%s\n",
				current->target);
			current->heredoc_fd = handle_heredoc_redir(current->target, shell);
			if (current->heredoc_fd == -2)
				break ;
		}
		current = current->next;
	}
	if (!current || (current && current->heredoc_fd != -2))
		// cuz of ((cat) << 1 << 2) case
		preprocess_all_heredocs(ast->s_subshell.child, shell);
}

static void	handle_heredoc_signal(t_ast *ast, t_shell *shell)
{
	t_redir	*current;

	current = NULL;
	if (ast->type == AST_SIMPLE_COMMAND && ast->s_simple_command.redirs)
	{
		current = ast->s_simple_command.redirs;
		while (current && current->next)
			current = current->next;
	}
	else if (ast->type == AST_SUBSHELL && ast->s_subshell.redirs)
	{
		current = ast->s_subshell.redirs;
		while (current && current->next)
			current = current->next;
	}
	if (current)
		current->heredoc_fd = -1;
	shell->status = 130;
	setup_signals(SIGNAL_NONE);
}

void	preprocess_all_heredocs(t_ast *ast, t_shell *shell)
{
	if (!ast)
		return ;
	if (ast->type == AST_SIMPLE_COMMAND && ast->s_simple_command.redirs)
		preprocess_heredoc_redirs(ast->s_simple_command.redirs, shell);
	else if (ast->type == AST_PIPELINE || ast->type == AST_LOGICAL_AND
		|| ast->type == AST_LOGICAL_OR)
	{
		preprocess_all_heredocs(ast->s_binary.left, shell);
		preprocess_all_heredocs(ast->s_binary.right, shell);
	}
	else if (ast->type == AST_SUBSHELL && ast->s_subshell.redirs)
		preprocess_subshell_heredocs(ast, shell);
	if (g_sig_value == SIGINT)
		handle_heredoc_signal(ast, shell);
}
