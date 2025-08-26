/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:46:59 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/26 14:34:27 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	advance(t_parser *parser)
{
	if (parser->current_token)
		parser->current_token = parser->current_token->next;
}

int	match(t_parser *parser, t_token_type type)
{
	return (parser && parser->current_token
		&& parser->current_token->type == type);
}

t_token	*expect(t_parser *parser, t_token_type type)
{
	t_token	*token;

	if (match(parser, type))
	{
		token = parser->current_token;
		advance(parser);
		return (token);
	}
	return (NULL);
}

void	set_parse_error(t_parser *p, char *msg)
{
	if (p->error)
		return ;
	p->error = 1;
	p->errmsg = msg;
}
