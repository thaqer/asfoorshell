/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_segments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:35:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/26 18:33:28 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_quote_character(t_segment **segments, char **start,
		char **current, t_state *state)
{
	int	result;

	if (**current == '\'')
	{
		result = handle_single_quote(segments, start, current, state);
		if (result == 0)
			return (0);
		if (result == 2)
			return (2);
	}
	else if (**current == '"')
	{
		result = handle_double_quote(segments, start, current, state);
		if (result == 0)
			return (0);
		if (result == 2)
			return (2);
	}
	else
		(*current)++;
	return (1);
}

t_segment	*parse_mixed_segments(char *input)
{
	t_segment	*segments;
	t_state		state;
	char		*start;
	char		*current;
	int			result;

	segments = NULL;
	state = STATE_NORMAL;
	start = input;
	current = input;
	while (*current)
	{
		result = process_quote_character(&segments, &start, &current, &state);
		if (result == 0)
			return (NULL);
		if (result == 2)
			continue ;
	}
	if (!handle_remaining_segment(&segments, start, current, state))
		return (NULL);
	return (segments);
}
