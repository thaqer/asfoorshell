/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:46:59 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/22 09:47:12 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stddef.h> // FOR NULL

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

// this version is lenient lets its caller deal with the error handling
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

/*
   expect() has two jobs:

   Ensure the current token is of a specific type (if not, handle error).
   Consume (advance past) that token.

   So there are two styles you can choose from:
   Option 1: void expect() (no return)
   Just checks and consumes.
   You use this when you don’t need the token itself (only its type).
example : expect(&parser, TOKEN_PIPE);
// At this point, we've verified there was a '|' and advanced.

Option 2: t_token *expect() (returns token)
Checks, consumes, and returns a pointer to the consumed token.
Useful when you need the token value 
(for example, to store a filename, argument, etc.).
Example: t_token *tok = expect(&parser, TOKEN_WORD);
add_argument(cmd, tok->value);
This avoids having to manually read p->current->value before advancing.

Which one is better?
For a shell parser, returning the token is more flexible, because:
Often you need to use the value:
For filenames after redirection
For command arguments
And sometimes you don’t care and can just ignore the return value.
Returning it doesn’t hurt; you can always ignore it.

Recommended Signature => 
t_token *expect(t_parser *parser, t_token_type type);
If you don’t need the token, 
just write => expect(&parser, TOKEN_PIPE); // ignore return
*/

/*
   Quick mental model
   - Use match() when you just want to check without consuming.
   - Use expect() when you want to check + consume.
   - Let expect() return the token so you can use it when needed.
   */
