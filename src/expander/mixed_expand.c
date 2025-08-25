/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mixed_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:51:31 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/25 21:37:54 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_splice_expansion(t_segment *segments, t_token **head,
		int index, t_expander_creds *expander_creds)
{
	t_token	*current;
	int		i;
	int		new_index;

	current = *head;
	i = 0;
	while (current && i < index)
	{
		current = current->next;
		i++;
	}
	if (!current || current->type != TOKEN_WORD)
	{
		free_segment_list(segments);
		return (0);
	}
	delete_node(head, current->value);
	new_index = index;
	if (!process_segments_with_splitting(segments, expander_creds, head,
			&new_index))
	{
		free_segment_list(segments);
		return (0);
	}
	return (1);
}

int	splice_mixed_expansion(t_token **head, int index, char *input,
		t_expander_creds *expander_creds)
{
	t_segment	*segments;

	segments = parse_mixed_segments(input);
	if (!segments)
		return (0);
	if (!execute_splice_expansion(segments, head, index, expander_creds))
		return (0);
	free_segment_list(segments);
	return (1);
}
