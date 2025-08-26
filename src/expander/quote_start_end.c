/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_start_end.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 22:00:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/25 22:05:55 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quote_start(t_quote_params *params, t_state new_state)
{
	char	*segment_value;

	if (*params->current > *params->start)
	{
		segment_value = ft_strndup(*params->start, *params->current
				- *params->start);
		if (!append_segment(params->segments, segment_value, SEGMENT_UNQUOTED))
		{
			free(segment_value);
			free_segment_list(*params->segments);
			return (0);
		}
		free(segment_value);
	}
	*params->start = *params->current;
	*params->state = new_state;
	(*params->current)++;
	return (1);
}

int	handle_quote_end(t_quote_params *params, t_segment_type seg_type)
{
	char	*segment_value;

	(*params->current)++;
	segment_value = ft_strndup(*params->start, *params->current
			- *params->start);
	if (!append_segment(params->segments, segment_value, seg_type))
	{
		free(segment_value);
		free_segment_list(*params->segments);
		return (0);
	}
	free(segment_value);
	*params->start = *params->current;
	*params->state = STATE_NORMAL;
	return (1);
}

static t_segment_type	get_segment_type(t_state state)
{
	if (state == STATE_NORMAL)
		return (SEGMENT_UNQUOTED);
	if (state == STATE_SQUOTE)
		return (SEGMENT_SINGLE_QUOTED);
	return (SEGMENT_DOUBLE_QUOTED);
}

int	handle_final_segment(t_segment **segments, char *start, char *current,
		t_state state)
{
	char			*segment_value;
	t_segment_type	seg_type;

	if (current > start)
	{
		seg_type = get_segment_type(state);
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
