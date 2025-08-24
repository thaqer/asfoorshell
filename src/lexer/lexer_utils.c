/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:49:56 by amaassaf          #+#    #+#             */
/*   Updated: 2025/08/24 12:48:21 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// this function copies n bytes of s into a newly allocated
// string (duplicate that accounts for a null character for termination -n + 1)
char	*ft_strndup(const char *s, size_t n)
{
	size_t	size;
	char	*duplicate;
	char	*saved_start;

	if (s == NULL)
		return (NULL);
	size = ++n;
	duplicate = malloc(size * sizeof(char));
	if (!duplicate)
		return (NULL);
	saved_start = duplicate;
	while (*s && --n)
		*duplicate++ = *s++;
	*duplicate = '\0';
	return (saved_start);
}

t_token	*create_token(char *value, t_token_type type, t_qflags flags)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->qflags = flags;
	token->next = NULL;
	return (token);
}

t_token	*append(t_token **head, char *value, t_token_type type, t_qflags flags)
{
	t_token	*new_token;
	t_token	*current;

	current = *head;
	new_token = create_token(value, type, flags);
	if (!new_token)
		return (NULL);
	if (!*head)
	{
		*head = new_token;
		return (new_token);
	}
	while (current->next)
		current = current->next;
	current->next = new_token;
	return (new_token);
}

void	free_tok_list(t_token *head)
{
	t_token	*current;

	while (head)
	{
		current = head;
		head = head->next;
		free(current->value);
		free(current);
	}
}

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
	while (*s)
	{
		if (*s == '\'')
		{
			has_sq = 1;
			s++;
			while (*s != '\'')
				s++;
			s++;
		}
		else if (*s == '"')
		{
			has_dq = 1;
			s++;
			while (*s != '"')
				s++;
			s++;
		}
		else
		{
			unquoted = 1;
			while (*s != '\'' && *s != '"' && *s)
				s++;
		}
	}
	if (has_sq)
	{
		flags |= QF_HAS_SQ;
	}
	if (has_dq)
	{
		flags |= QF_HAS_DQ;
	}
	if (has_sq && !has_dq && !unquoted)
	{
		flags |= QF_ALL_SQ;
	}
	else if (!has_sq && has_dq && !unquoted)
	{
		flags |= QF_ALL_DQ;
	}
	if ((has_sq && has_dq) || ((has_sq || has_dq) && unquoted))
	{
		flags |= QF_MIXED;
	}
	return (flags);
}
