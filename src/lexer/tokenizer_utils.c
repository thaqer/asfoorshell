/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 05:10:00 by daniah            #+#    #+#             */
/*   Updated: 2025/08/27 17:42:16 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

t_separator_type	handle_double_char_sep(char *current_position)
{
	if (*current_position == '>')
		return (SEP_APPEND);
	else if (*current_position == '<')
		return (SEP_HEREDOC);
	else if (*current_position == '&')
		return (SEP_AND);
	else if (*current_position == '|')
		return (SEP_OR);
	else if (*current_position == '\'')
		return (SEP_SQUOTE);
	else if (*current_position == '\"')
		return (SEP_DQUOTE);
	else
		return (SEP_NONE);
}

t_separator_type	is_separator(char *current_position)
{
	if (*current_position && *(current_position + 1)
		&& *current_position == *(current_position + 1)
		&& *current_position != ' ')
		return (handle_double_char_sep(current_position));
	if (*current_position == ' ')
		return (SEP_SPACE);
	else if (*current_position == '|')
		return (SEP_PIPE);
	else if (*current_position == '>')
		return (SEP_REDIRECT_OUT);
	else if (*current_position == '<')
		return (SEP_REDIRECT_IN);
	else if (*current_position == '(')
		return (SEP_OPEN_PAREN);
	else if (*current_position == ')')
		return (SEP_CLOSE_PAREN);
	else if (*current_position == '\'')
		return (SEP_SQUOTE);
	else if (*current_position == '\"')
		return (SEP_DQUOTE);
	else
		return (SEP_NONE);
}

void	append_separator(t_token **head, t_separator_type type,
		char **input_ref, char **start_ref)
{
	char			*value;
	t_token_type	tok_type;

	tok_type = type_determinant(type);
	*start_ref = *input_ref;
	if (type == SEP_APPEND || type == SEP_HEREDOC || type == SEP_AND
		|| type == SEP_OR)
		*input_ref += 2;
	else
		(*input_ref)++;
	value = ft_strndup(*start_ref, (*input_ref - *start_ref));
	*start_ref = *input_ref;
	if (!append(head, value, tok_type, QF_NONE))
	{
		if (value)
			free(value);
		free_tok_list(*head);
		*head = NULL;
		exit(EXIT_FAILURE);
	}
	free(value);
	value = NULL;
}

t_token_type	type_determinant(t_separator_type sep_type)
{
	if (sep_type == SEP_APPEND)
		return (TOKEN_APPEND);
	else if (sep_type == SEP_HEREDOC)
		return (TOKEN_HEREDOC);
	else if (sep_type == SEP_REDIRECT_OUT)
		return (TOKEN_REDIRECT_OUT);
	else if (sep_type == SEP_REDIRECT_IN)
		return (TOKEN_REDIRECT_IN);
	else if (sep_type == SEP_PIPE)
		return (TOKEN_PIPE);
	else if (sep_type == SEP_AND)
		return (TOKEN_AND);
	else if (sep_type == SEP_OR)
		return (TOKEN_OR);
	else if (sep_type == SEP_OPEN_PAREN)
		return (TOKEN_OPEN_PAREN);
	else
		return (TOKEN_CLOSE_PAREN);
}

t_token	*get_prev_token(t_token *head)
{
	t_token	*cur;

	if (!head)
		return (NULL);
	cur = head;
	while (cur && cur->next && cur->next->next)
		cur = cur->next;
	if (!cur || !cur->next)
		return (NULL);
	return (cur);
}
