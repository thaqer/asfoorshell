/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 22:15:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/25 22:14:45 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_logical_error(t_parser *parser)
{
	if (match(parser, TOKEN_AND) || match(parser, TOKEN_OR))
	{
		set_parse_error(parser,
			"minishell:expected cmd after logic operator");
		return (1);
	}
	return (0);
}

int	handle_parse_logical_error(t_parser *parser, t_ast *left)
{
	if (parser->error || !left)
	{
		if (check_logical_error(parser))
			return (1);
		return (1);
	}
	return (0);
}

static int	check_pipe_error(t_parser *parser)
{
	if (match(parser, TOKEN_PIPE))
	{
		set_parse_error(parser,
			"minishell: syntax error: expected command after pipe");
		return (1);
	}
	return (0);
}

int	handle_parse_pipeline_error(t_parser *parser, t_ast *left)
{
	if (parser->error || !left)
	{
		if (check_pipe_error(parser))
			return (1);
		return (1);
	}
	return (0);
}
