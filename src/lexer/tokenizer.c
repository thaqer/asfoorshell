/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:26:32 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/26 20:03:46 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

static void	handle_single_quote_utils(t_state *state, char **s_ref)
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

static void	handle_double_quote_utils(t_state *state, char **s_ref)
{
	++(*s_ref);
	if (*state == STATE_NORMAL && **s_ref == '\"')
	{
		++(*s_ref);
		if (**s_ref == '\"')
			--(*s_ref);
	}
	else if (*state == STATE_DQUOTE && **s_ref != '\"')
		*state = STATE_NORMAL;
	else
		*state = STATE_DQUOTE;
	if (*state == STATE_NORMAL && **s_ref == '\'')
		*state = STATE_SQUOTE;
}

void	state_determinant(t_state *state, char **s_ref)
{
	if (**s_ref == '\'' && *state != STATE_DQUOTE)
		handle_single_quote_utils(state, s_ref);
	else if (**s_ref == '\"' && *state != STATE_SQUOTE)
		handle_double_quote_utils(state, s_ref);
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
