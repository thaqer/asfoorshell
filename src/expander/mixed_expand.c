/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mixed_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:51:31 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/08/25 00:30:10 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_segment	*create_segment(char *value, t_segment_type type)
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

/* Append segment to list */
static t_segment	*append_segment(t_segment **head, char *value,
		t_segment_type type)
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

static void	free_segment_list(t_segment *head)
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

static t_segment	*parse_mixed_segments(char *input)
{
	t_segment		*segments;
	t_state			state;
	char			*start;
	char			*current;
	char			*segment_value;
	t_segment_type	seg_type;

	segments = NULL;
	state = STATE_NORMAL;
	start = input;
	current = input;
	while (*current)
	{
		if (*current == '\'' && state == STATE_NORMAL)
		{
			if (current > start)
			{
				segment_value = ft_strndup(start, current - start);
				if (!append_segment(&segments, segment_value, SEGMENT_UNQUOTED))
				{
					free(segment_value);
					free_segment_list(segments);
					return (NULL);
				}
				free(segment_value);
			}
			start = current;
			state = STATE_SQUOTE;
			current++;
		}
		else if (*current == '\'' && state == STATE_SQUOTE)
		{
			current++;
			segment_value = ft_strndup(start, current - start);
			if (!append_segment(&segments, segment_value,
					SEGMENT_SINGLE_QUOTED))
			{
				free(segment_value);
				free_segment_list(segments);
				return (NULL);
			}
			free(segment_value);
			start = current;
			state = STATE_NORMAL;
			continue ;
		}
		else if (*current == '"' && state == STATE_NORMAL)
		{
			if (current > start)
			{
				segment_value = ft_strndup(start, current - start);
				if (!append_segment(&segments, segment_value, SEGMENT_UNQUOTED))
				{
					free(segment_value);
					free_segment_list(segments);
					return (NULL);
				}
				free(segment_value);
			}
			start = current;
			state = STATE_DQUOTE;
			current++;
		}
		else if (*current == '"' && state == STATE_DQUOTE)
		{
			current++;
			segment_value = ft_strndup(start, current - start);
			if (!append_segment(&segments, segment_value,
					SEGMENT_DOUBLE_QUOTED))
			{
				free(segment_value);
				free_segment_list(segments);
				return (NULL);
			}
			free(segment_value);
			start = current;
			state = STATE_NORMAL;
			continue ;
		}
		else
		{
			current++;
		}
	}
	if (current > start)
	{
		seg_type = (state == STATE_NORMAL) ? SEGMENT_UNQUOTED : (state == STATE_SQUOTE) ? SEGMENT_SINGLE_QUOTED : SEGMENT_DOUBLE_QUOTED;
		segment_value = ft_strndup(start, current - start);
		if (!append_segment(&segments, segment_value, seg_type))
		{
			free(segment_value);
			free_segment_list(segments);
			return (NULL);
		}
		free(segment_value);
	}
	return (segments);
}

static int	append_split_tokens(t_token **head, int *index, char *str)
{
	char	**split;
	int		i;
	t_token	*new_token;

	if (!str || !*str)
		return (1);
	split = ft_split(str, ' ');
	if (!split)
		return (0);
	i = 0;
	while (split[i])
	{
		new_token = create_token(split[i], TOKEN_WORD, QF_NONE);
		if (!new_token)
		{
			while (split[i])
				free(split[i++]);
			free(split);
			return (0);
		}
		if (!insert_at_index(head, *index, new_token))
		{
			free(new_token->value);
			free(new_token);
			while (split[i])
				free(split[i++]);
			free(split);
			return (0);
		}
		(*index)++;
		i++;
	}
	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
	return (1);
}

static int	has_quoted_segments(t_segment *segments)
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

static int	process_segments_with_splitting(t_segment *segments,
		t_expander_creds *expander_creds, t_token **head, int *index)
{
	t_segment	*current;
	char		*segment_result;
	char		*accumulated;
	int			has_accumulated;
	int			contains_quotes;
	char		*temp;

	current = segments;
	accumulated = ft_strdup("");
	has_accumulated = 0;
	contains_quotes = has_quoted_segments(segments);
	if (!accumulated)
		return (0);
	while (current)
	{
		segment_result = NULL;
		if (current->type == SEGMENT_UNQUOTED)
		{
			segment_result = unquoted_variable_handler(current->value,
					expander_creds);
			if (!segment_result)
			{
				free(accumulated);
				return (0);
			}
			if (contains_quotes)
			{
				temp = ft_strjoin(accumulated, segment_result);
				free(accumulated);
				free(segment_result);
				if (!temp)
					return (0);
				accumulated = temp;
				has_accumulated = 1;
			}
			else
			{
				if (has_accumulated && *accumulated)
				{
					if (!append_split_tokens(head, index, accumulated))
					{
						free(accumulated);
						free(segment_result);
						return (0);
					}
					free(accumulated);
					accumulated = ft_strdup("");
					if (!accumulated)
					{
						free(segment_result);
						return (0);
					}
					has_accumulated = 0;
				}
				if (!append_split_tokens(head, index, segment_result))
				{
					free(accumulated);
					free(segment_result);
					return (0);
				}
				free(segment_result);
			}
		}
		else
		{
			if (current->type == SEGMENT_SINGLE_QUOTED)
				segment_result = strip_quotes(current->value);
			else if (current->type == SEGMENT_DOUBLE_QUOTED)
				segment_result = dquote_expander(current->value,
						expander_creds);
			if (!segment_result)
			{
				free(accumulated);
				return (0);
			}
			temp = ft_strjoin(accumulated, segment_result);
			free(accumulated);
			free(segment_result);
			if (!temp)
				return (0);
			accumulated = temp;
			has_accumulated = 1;
		}
		current = current->next;
	}
	if (has_accumulated && *accumulated)
	{
		if (!append_split_tokens(head, index, accumulated))
		{
			free(accumulated);
			return (0);
		}
	}
	free(accumulated);
	return (1);
}

int	splice_mixed_expansion(t_token **head, int index, char *input,
		t_expander_creds *expander_creds)
{
	t_segment	*segments;
	t_token		*current;
	int			i;
	int			new_index;

	segments = parse_mixed_segments(input);
	if (!segments)
		return (0);
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
	free_segment_list(segments);
	return (1);
}
