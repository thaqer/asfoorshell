/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:55:00 by daniah            #+#    #+#             */
/*   Updated: 2025/08/25 11:01:17 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_size(t_token *head)
{
	int		count;
	t_token	*current;

	count = 0;
	current = head;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	delete_node(t_token **head, const char *value)
{
	t_token	*current;
	t_token	*prev;

	if (head == NULL || *head == NULL)
		return ;
	current = *head;
	prev = NULL;
	while (current != NULL)
	{
		if (ft_strncmp(current->value, value, ft_strlen(value)) == 0)
		{
			if (prev == NULL)
				*head = current->next;
			else
				prev->next = current->next;
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	insert_at_index(t_token **head, size_t index, t_token *new_node)
{
	size_t	i;
	t_token	*cur;

	if (!head || !new_node)
		return (0);
	if (index == 0)
	{
		new_node->next = *head;
		*head = new_node;
		return (1);
	}
	i = 0;
	cur = *head;
	while (cur && i + 1 < index)
	{
		cur = cur->next;
		i++;
	}
	if (!cur)
		return (0);
	new_node->next = cur->next;
	cur->next = new_node;
	return (1);
}

static void	scan_quotes(char **s, int *has_sq, int *has_dq, int *unquoted)
{
	while (**s)
	{
		if (**s == '\'')
		{
			*has_sq = 1;
			(*s)++;
			while (**s != '\'')
				(*s)++;
			(*s)++;
		}
		else if (**s == '"')
		{
			*has_dq = 1;
			(*s)++;
			while (**s != '"')
				(*s)++;
			(*s)++;
		}
		else
		{
			*unquoted = 1;
			while (**s != '\'' && **s != '"' && **s)
				(*s)++;
		}
	}
}

t_qflags	compute_qflags_from_raw(char *s)
{
	int			has_sq;
	int			has_dq;
	int			unquoted;
	t_qflags	flags;

	has_sq = 0;
	has_dq = 0;
	unquoted = 0;
	flags = QF_NONE;
	scan_quotes(&s, &has_sq, &has_dq, &unquoted);
	if (has_sq)
		flags |= QF_HAS_SQ;
	if (has_dq)
		flags |= QF_HAS_DQ;
	if (has_sq && !has_dq && !unquoted)
		flags |= QF_ALL_SQ;
	else if (!has_sq && has_dq && !unquoted)
		flags |= QF_ALL_DQ;
	if ((has_sq && has_dq) || ((has_sq || has_dq) && unquoted))
		flags |= QF_MIXED;
	return (flags);
}
