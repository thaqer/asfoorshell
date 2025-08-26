/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_dispatcher.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:46:29 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/22 09:46:36 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_group(t_parser *parser)
{
	t_ast	*inner;

	inner = parse_logical_line(parser);
	if (parser->error || !inner)
		return (NULL);
	if (!expect(parser, TOKEN_CLOSE_PAREN))
	{
		free_ast(inner);
		set_parse_error(parser, "minishell: syntax error: expected ')'");
		return (NULL);
	}
	return (inner);
}

t_ast	*clean_group(t_ast *inner, t_redir *redirs)
{
	free_ast(inner);
	free_redirs(redirs);
	return (NULL);
}

t_ast	*parse_command(t_parser *parser)
{
	t_redir	*redirs;
	t_ast	*inner;

	redirs = NULL;
	inner = NULL;
	if (match(parser, TOKEN_OPEN_PAREN))
	{
		advance(parser);
		inner = parse_group(parser);
		if (parser->error || !inner)
			return (NULL);
		while (!parser->error && is_redirection_tok(parser->current_token))
		{
			if (parse_redirections(parser, &redirs) < 0)
				return (clean_group(inner, redirs));
		}
		if (parser->error)
			return (clean_group(inner, redirs));
		return (ast_create_subshell(inner, redirs));
	}
	return (parse_simple_command(parser));
}
