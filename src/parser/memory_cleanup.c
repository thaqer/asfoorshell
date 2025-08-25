/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:46:09 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/25 13:02:11 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_redir *redirs)
{
	t_redir	*next;

	while (redirs)
	{
		next = redirs->next;
		free(redirs->target);
		if (redirs->heredoc_fd > 0)
			close(redirs->heredoc_fd);
		free(redirs);
		redirs = next;
	}
}

// When failing mid simple_command
/* frees argv[0..argc-1] and the argv vector itself */
void	free_partial_arguments(char **argv, int argc)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (i < argc)
	{
		free(argv[i++]);
	}
	free(argv);
}

/* frees a full NULL-terminated argv */
void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i++]);
	}
	free(argv);
}

/* recursive AST free (sketch) */
void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == AST_SIMPLE_COMMAND)
	{
		free_argv(node->s_simple_command.argv);
		free_redirs(node->s_simple_command.redirs);
	}
	else if (node->type == AST_PIPELINE || node->type == AST_LOGICAL_AND
		|| node->type == AST_LOGICAL_OR)
	{
		free_ast(node->s_binary.left);
		free_ast(node->s_binary.right);
	}
	else if (node->type == AST_SUBSHELL)
	{
		free_ast(node->s_subshell.child);
		free_redirs(node->s_subshell.redirs);
	}
	free(node);
}
