/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:47:19 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/22 09:47:33 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection_tok(t_token *token)
{
	t_token_type	type;

	if (!token)
		return (0);
	type = token->type;
	return (type == TOKEN_REDIRECT_IN
		|| type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND
		|| type == TOKEN_HEREDOC);
}

t_redir_type	token_to_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN)
		return (REDIR_IN);
	if (type == TOKEN_REDIRECT_OUT)
		return (REDIR_OUT);
	if (type == TOKEN_APPEND)
		return (REDIR_APPEND);
	return (REDIR_HEREDOC);
}

int	add_redirection(t_redir **head, t_token_type type, const char *target)
{
	t_redir	*node;
	t_redir	*current;

	node = ft_calloc(1, sizeof(*node));
	if (!node)
		return (-1);
	node->type = token_to_redir_type(type);
	node->target = ft_strdup(target);
	node->heredoc_fd = -1;
	if (!node->target)
	{
		free(node);
		return (-1);
	}
	if (*head == NULL)
		*head = node;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = node;
	}
	return (0);
}

int	parse_redirections(t_parser *parser, t_redir **redirs)
{
	t_token			*target;
	t_token_type	type;

	type = parser->current_token->type;
	advance(parser);
	target = expect(parser, TOKEN_WORD);
	if (!target)
	{
		set_parse_error(parser,
				"minishell: syntax error: expected filename after redirection");
		return (-1);
	}
	if (add_redirection(redirs, type, target->value) < 0)
	{
		set_parse_error(parser,
				"minishell: memory allocation failed in redirection");
		return (-1);
	}
	(*redirs)->s_delim_quoted = false;
	if (type == TOKEN_HEREDOC)
	{
		if (target->qflags & QF_HD_QUOTED)
			(*redirs)->s_delim_quoted = true;
	}
	return (0);
}
