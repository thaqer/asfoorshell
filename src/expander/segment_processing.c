/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_processing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:35:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/25 22:09:59 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_single_segment(t_segment *current,
		t_expander_creds *expander_creds, int *flag)
{
	char	*segment_result;

	segment_result = NULL;
	if (current->type == SEGMENT_SINGLE_QUOTED)
	{
		segment_result = strip_quotes(current->value);
		if (segment_result)
			*flag = 1;
	}
	else if (current->type == SEGMENT_DOUBLE_QUOTED)
	{
		segment_result = dquote_expander(current->value, expander_creds);
	}
	else if (current->type == SEGMENT_UNQUOTED)
	{
		segment_result = unquoted_variable_handler(current->value,
				expander_creds);
	}
	return (segment_result);
}

int	accumulate_segment_result(char **accumulated, char *segment_result)
{
	char	*temp;

	if (segment_result)
	{
		temp = ft_strjoin(*accumulated, segment_result);
		free(*accumulated);
		free(segment_result);
		if (!temp)
			return (0);
		*accumulated = temp;
		return (1);
	}
	return (1);
}

int	process_segment_loop(t_segment *segments, t_expander_creds *expander_creds,
		char **accumulated, int *flag)
{
	t_segment	*current;
	char		*segment_result;

	current = segments;
	while (current)
	{
		segment_result = process_single_segment(current, expander_creds, flag);
		if (current->type == SEGMENT_SINGLE_QUOTED && !segment_result)
		{
			free(*accumulated);
			return (0);
		}
		if (!accumulate_segment_result(accumulated, segment_result))
		{
			free(*accumulated);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	finalize_tokens(char *accumulated, t_token **head, int *index)
{
	if (accumulated && *accumulated)
	{
		if (!append_split_tokens(head, index, accumulated))
			return (0);
	}
	return (1);
}

int	process_segments_with_splitting(t_segment *segments,
		t_expander_creds *expander_creds, t_token **head, int *index)
{
	char	*accumulated;
	int		flag;

	accumulated = ft_strdup("");
	flag = 0;
	if (!accumulated)
		return (0);
	if (!process_segment_loop(segments, expander_creds, &accumulated, &flag))
		return (0);
	if (!finalize_tokens(accumulated, head, index))
	{
		free(accumulated);
		return (0);
	}
	free(accumulated);
	return (1);
}
