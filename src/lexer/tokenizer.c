/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:26:32 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/24 12:07:44 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include <stdio.h>

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
	else
		return (SEP_NONE);
}

t_separator_type	is_separator(char *current_position)
{
	if (*current_position && *current_position == *(current_position + 1)
		&& *current_position != ' ')
		return (handle_double_char_sep(current_position));
	if (*current_position == ' ')
		return (SEP_SPACE);
	else if (*current_position == '|')
		return (SEP_PIPE);
	else if (*current_position == '\'')
		return (SEP_SQUOTE);
	else if (*current_position == '\"')
		return (SEP_DQUOTE);
	else if (*current_position == '>')
		return (SEP_REDIRECT_OUT);
	else if (*current_position == '<')
		return (SEP_REDIRECT_IN);
	else if (*current_position == '(')
		return (SEP_OPEN_PAREN);
	else if (*current_position == ')')
		return (SEP_CLOSE_PAREN);
	else
		return (SEP_NONE);
}

void	state_determinant(t_state *state, char **s_ref)
{
	if (**s_ref == '\'' && *state != STATE_DQUOTE)
	{
		++(*s_ref);
		if (*state == STATE_NORMAL && **s_ref == '\'')
			++(*s_ref);
		else if (*state == STATE_SQUOTE && **s_ref != '\'')
			*state = STATE_NORMAL;
		else
			*state = STATE_SQUOTE;
		if (*state == STATE_NORMAL && **s_ref == '\"')
			*state = STATE_DQUOTE;
	}
	else if (**s_ref == '\"' && *state != STATE_SQUOTE)
	{
		++(*s_ref);
		if (*state == STATE_NORMAL && **s_ref == '\"')
			++(*s_ref);
		else if (*state == STATE_DQUOTE && **s_ref != '\"')
			*state = STATE_NORMAL;
		else
			*state = STATE_DQUOTE;
		if (*state == STATE_NORMAL && **s_ref == '\'')
			*state = STATE_SQUOTE;
	}
}

static t_token	*get_prev_token(t_token *head)
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

void	append_and_handle_failure(t_token **token_list, char *start_ref,
		char *input_ref)
{
	char		*value;
	t_qflags	flags;
	t_token		*prev;

	value = ft_strndup(start_ref, (input_ref - start_ref));
	flags = compute_qflags_from_raw(value);
	if (!append(token_list, value, TOKEN_WORD, flags))
	{
		free(value);
		free_tok_list(*token_list);
		exit(EXIT_FAILURE);
	}
	free(value);
	prev = get_prev_token(*token_list);
	if (!prev)
		return ;
	if (prev->type == TOKEN_HEREDOC)
	{
		if (prev->next->qflags & (QF_HAS_SQ | QF_HAS_DQ))
			prev->next->qflags |= QF_HD_QUOTED;
	}
}

void	normal_state_handler(t_separator_type sep_type, t_token **token_list,
		char **start_ref, char **input_ref)
{
	if (sep_type == SEP_NONE)
	{
		if (**input_ref)
			(*input_ref)++;
		if (!**input_ref)
			append_and_handle_failure(token_list, *start_ref, *input_ref);
	}
	else
	{
		if ((*input_ref - *start_ref) != 0)
		{
			append_and_handle_failure(token_list, *start_ref, *input_ref);
		}
		if (sep_type != SEP_SPACE)
		{
			append_separator(token_list, sep_type, input_ref, start_ref);
		}
		else
		{
			*start_ref = ++(*input_ref);
		}
	}
}

static t_token	*tokenize_norm_utils(t_token *token_list)
{
	ft_putstr_fd("minishell: syntax error: unclosed quote \n", 2);
	free_tok_list(token_list);
	return (NULL);
}

t_token	*tokenize(char *input)
{
	t_token				*token_list;
	char				*start;
	t_separator_type	sep_type;
	t_state				state;

	token_list = NULL;
	if (!input)
		return (NULL);
	while (*input == ' ' || (*input >= 9 && *input <= 13))
		input++;
	start = input;
	state = STATE_NORMAL;
	while (*input)
	{
		state_determinant(&state, &input);
		sep_type = is_separator(input);
		if (state == STATE_NORMAL)
			normal_state_handler(sep_type, &token_list, &start, &input);
		else
			input++;
	}
	if (state != STATE_NORMAL)
		return (tokenize_norm_utils(token_list));
	return (token_list);
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
