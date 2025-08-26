/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_segments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:35:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/26 20:24:29 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_unquoted_before_sq(t_segment **segments, char **start,
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

static int	handle_squote_end(t_segment **segments, char **start,
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

int	handle_single_quote(t_segment **segments, char **start,
		char **current, t_state *state)
{
	if (*state == STATE_NORMAL)
	{
		if (!handle_unquoted_before_sq(segments, start, current))
			return (0);
		*start = *current;
		*state = STATE_SQUOTE;
		(*current)++;
	}
	else if (*state == STATE_SQUOTE)
	{
		return (handle_squote_end(segments, start, current, state));
	}
	else if (*state == STATE_DQUOTE)
		(*current)++;
	return (1);
}

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
