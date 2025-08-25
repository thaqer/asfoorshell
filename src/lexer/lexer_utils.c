/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:49:56 by amaassaf          #+#    #+#             */
/*   Updated: 2025/08/25 22:08:06 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
