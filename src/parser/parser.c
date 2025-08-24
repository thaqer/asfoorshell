/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:46:43 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/22 09:46:53 by tbaniatt         ###   ########.fr       */
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

t_ast	*parse_logical_line(t_parser *parser)
{
	t_ast			*left;
	t_ast			*right;
	t_token_type	op;

	left = parse_pipeline(parser);
	if (parser->error || !left)
	{
		if (match(parser, TOKEN_AND) || match(parser, TOKEN_OR))
			set_parse_error(parser,
				"minishell:expected cmd after logic operator");
		return (NULL);
	}
	while (!parser->error && (match(parser, TOKEN_AND) || match(parser,
				TOKEN_OR)))
	{
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
	}
	if (parser->error)
		return (NULL);
	return (left);
}

t_ast	*parse_pipeline(t_parser *parser)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command(parser);
	if (parser->error || !left)
	{
		if (match(parser, TOKEN_PIPE))
			set_parse_error(parser,
				"minishell: syntax error: expected command after pipe");
		return (NULL);
	}
	while (!parser->error && match(parser, TOKEN_PIPE))
	{
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
	}
	if (parser->error)
		return (NULL);
	return (left);
}
