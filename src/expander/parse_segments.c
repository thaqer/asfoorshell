/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_segments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:35:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/25 21:37:54 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		result = process_character(&segments, &start, &current, &state);
		if (result == 0)
			return (NULL);
		if (result == 2)
			continue ;
	}
	if (!handle_final_segment(&segments, start, current, state))
		return (NULL);
	return (segments);
}
