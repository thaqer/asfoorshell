/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 22:00:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/25 22:05:55 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quote_character(t_quote_params *params, char quote_char)
{
	if (quote_char == '\'' && *params->state == STATE_NORMAL)
	{
		if (!handle_quote_start(params, STATE_SQUOTE))
			return (0);
	}
	else if (quote_char == '\'' && *params->state == STATE_SQUOTE)
	{
		if (!handle_quote_end(params, SEGMENT_SINGLE_QUOTED))
			return (0);
		return (2);
	}
	else if (quote_char == '"' && *params->state == STATE_NORMAL)
	{
		if (!handle_quote_start(params, STATE_DQUOTE))
			return (0);
	}
	else if (quote_char == '"' && *params->state == STATE_DQUOTE)
	{
		if (!handle_quote_end(params, SEGMENT_DOUBLE_QUOTED))
			return (0);
		return (2);
	}
	return (1);
}

int	process_character(t_segment **segments, char **start,
		char **current, t_state *state)
{
	t_quote_params	params;
	int				result;

	if (**current == '\'' || **current == '"')
	{
		params.segments = segments;
		params.start = start;
		params.current = current;
		params.state = state;
		result = handle_quote_character(&params, **current);
		if (result == 0)
			return (0);
		if (result == 2)
			return (2);
	}
	else
	{
		(*current)++;
	}
	return (1);
}
