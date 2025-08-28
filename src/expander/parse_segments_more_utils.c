/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_segments_more_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 18:30:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/26 20:28:20 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_unquoted_before_dq(t_segment **segments, char **start,
		char **current)
{
	char	*segment_value;

	if (*current > *start)
	{
		segment_value = ft_strndup(*start, *current - *start);
		if (!append_segment(segments, segment_value, SEGMENT_UNQUOTED))
		{
			free(segment_value);
			free_segment_list(*segments);
			return (0);
		}
		free(segment_value);
	}
	return (1);
}

static int	handle_dquote_end(t_segment **segments, char **start,
		char **current, t_state *state)
{
	char	*segment_value;

	(*current)++;
	segment_value = ft_strndup(*start, *current - *start);
	if (!append_segment(segments, segment_value, SEGMENT_DOUBLE_QUOTED))
	{
		free(segment_value);
		free_segment_list(*segments);
		return (0);
	}
	free(segment_value);
	*start = *current;
	*state = STATE_NORMAL;
	return (2);
}

int	handle_double_quote(t_segment **segments, char **start, char **current,
		t_state *state)
{
	if (*state == STATE_NORMAL)
	{
		if (!handle_unquoted_before_dq(segments, start, current))
			return (0);
		*start = *current;
		*state = STATE_DQUOTE;
		(*current)++;
	}
	else if (*state == STATE_DQUOTE)
	{
		return (handle_dquote_end(segments, start, current, state));
	}
	else if (*state == STATE_SQUOTE)
		(*current)++;
	return (1);
}
