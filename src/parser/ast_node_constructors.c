/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_constructors.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:45:52 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/22 09:46:03 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*ast_create_simple(char **argv, t_redir *redirs)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = AST_SIMPLE_COMMAND;
	node->s_simple_command.argv = argv;
	node->s_simple_command.redirs = redirs;
	return (node);
}

t_ast	*ast_create_binary(t_ast_node_type type, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->s_binary.left = left;
	node->s_binary.right = right;
	return (node);
}

t_ast	*ast_create_subshell(t_ast *child, t_redir *redirs)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = AST_SUBSHELL;
	node->s_subshell.child = child;
	node->s_subshell.redirs = redirs;
	return (node);
}
