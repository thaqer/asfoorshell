/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_final_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:59:37 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/27 17:37:15 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	normal_state_handler(t_separator_type sep_type, t_token **token_list,
		char **start_ref, char **input_ref)
{
	if (sep_type == SEP_NONE || sep_type == SEP_SQUOTE
		|| sep_type == SEP_DQUOTE)
	{
		if (**input_ref && sep_type == SEP_NONE)
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
