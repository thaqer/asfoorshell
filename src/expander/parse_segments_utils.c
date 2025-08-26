/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_segments_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:30:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/26 18:33:28 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_unquoted_before_sq(t_segment **segments, char **start,
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

int	handle_squote_end(t_segment **segments, char **start,
		char **current, t_state *state)
{
	char	*segment_value;

	(*current)++;
	segment_value = ft_strndup(*start, *current - *start);
	if (!append_segment(segments, segment_value, SEGMENT_SINGLE_QUOTED))
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

int	handle_unquoted_before_dq(t_segment **segments, char **start,
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

int	handle_dquote_end(t_segment **segments, char **start,
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

int	handle_remaining_segment(t_segment **segments, char *start,
		char *current, t_state state)
{
	t_segment_type	seg_type;
	char			*segment_value;

	if (current > start)
	{
		if (state == STATE_NORMAL)
			seg_type = SEGMENT_UNQUOTED;
		else if (state == STATE_SQUOTE)
			seg_type = SEGMENT_SINGLE_QUOTED;
		else
			seg_type = SEGMENT_DOUBLE_QUOTED;
		segment_value = ft_strndup(start, current - start);
		if (!append_segment(segments, segment_value, seg_type))
		{
			free(segment_value);
			free_segment_list(*segments);
			return (0);
		}
		free(segment_value);
	}
	return (1);
}
