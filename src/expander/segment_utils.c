/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:35:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/26 20:31:35 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_segment	*create_segment(char *value, t_segment_type type)
{
	t_segment	*segment;

	if (!value)
		return (NULL);
	segment = malloc(sizeof(t_segment));
	if (!segment)
		return (NULL);
	segment->value = ft_strdup(value);
	if (!segment->value)
	{
		free(segment);
		return (NULL);
	}
	segment->type = type;
	segment->next = NULL;
	return (segment);
}

t_segment	*append_segment(t_segment **head, char *value, t_segment_type type)
{
	t_segment	*new_segment;
	t_segment	*current;

	new_segment = create_segment(value, type);
	if (!new_segment)
		return (NULL);
	if (!*head)
	{
		*head = new_segment;
		return (new_segment);
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_segment;
	return (new_segment);
}

void	free_segment_list(t_segment *head)
{
	t_segment	*current;

	while (head)
	{
		current = head;
		head = head->next;
		free(current->value);
		free(current);
	}
}

int	has_quoted_segments(t_segment *segments)
{
	t_segment	*current;

	current = segments;
	while (current)
	{
		if (current->type == SEGMENT_SINGLE_QUOTED
			|| current->type == SEGMENT_DOUBLE_QUOTED)
			return (1);
		current = current->next;
	}
	return (0);
}

char	*strip_all_outer_quotes(char *str)
{
	char	*current;
	char	*temp;
	size_t	len;

	if (!str)
		return (NULL);
	current = ft_strdup(str);
	if (!current)
		return (NULL);
	while (current)
	{
		len = ft_strlen(current);
		if (len < 2)
			break ;
		if ((current[0] == '"' && current[len - 1] == '"')
			|| (current[0] == '\'' && current[len - 1] == '\''))
		{
			temp = ft_strndup(current + 1, len - 2);
			free(current);
			current = temp;
		}
		else
			break ;
	}
	return (current);
}
