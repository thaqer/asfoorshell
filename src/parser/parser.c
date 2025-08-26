/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:46:43 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/25 22:14:45 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_input_line(t_parser *parser)
{
	t_ast	*root;

	root = parse_logical_line(parser);
	if (!root || parser->error)
	{
		if (parser->errmsg)
			ft_putendl_fd(parser->errmsg, 2);
		else
			ft_putendl_fd("minishell: syntax error", 2);
		free_ast(root);
		return (NULL);
	}
	return (root);
}

static t_ast	*handle_logical_operator(t_parser *parser, t_ast *left)
{
	t_ast			*right;
	t_token_type	op;

	op = parser->current_token->type;
	advance(parser);
	right = parse_pipeline(parser);
	if (parser->error || !right)
	{
		set_parse_error(parser,
			"minishell:expected cmd after logic operator");
		free_ast(left);
		return (NULL);
	}
	if (op == TOKEN_AND)
		left = ast_create_binary(AST_LOGICAL_AND, left, right);
	else
		left = ast_create_binary(AST_LOGICAL_OR, left, right);
	return (left);
}

t_ast	*parse_logical_line(t_parser *parser)
{
	t_ast	*left;

	left = parse_pipeline(parser);
	if (handle_parse_logical_error(parser, left))
		return (NULL);
	while (!parser->error && (match(parser, TOKEN_AND) || match(parser,
				TOKEN_OR)))
	{
		left = handle_logical_operator(parser, left);
		if (!left)
			return (NULL);
	}
	if (parser->error)
		return (NULL);
	return (left);
}

static t_ast	*handle_pipe_operator(t_parser *parser, t_ast *left)
{
	t_ast	*right;

	advance(parser);
	right = parse_command(parser);
	if (parser->error || !right)
	{
		set_parse_error(parser,
			"minishell: syntax error: expected command after pipe");
		free_ast(left);
		return (NULL);
	}
	left = ast_create_binary(AST_PIPELINE, left, right);
	return (left);
}

t_ast	*parse_pipeline(t_parser *parser)
{
	t_ast	*left;

	left = parse_command(parser);
	if (handle_parse_pipeline_error(parser, left))
		return (NULL);
	while (!parser->error && match(parser, TOKEN_PIPE))
	{
		left = handle_pipe_operator(parser, left);
		if (!left)
			return (NULL);
	}
	if (parser->error)
		return (NULL);
	return (left);
}
