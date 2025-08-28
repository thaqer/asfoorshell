/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:49:56 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/27 12:07:45 by dqaddomi         ###   ########.fr       */
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

void	append_and_handle_failure(t_token **token_list, char *start_ref,
		char *input_ref)
{
	char		*value;
	t_qflags	flags;
	t_token		*prev;

	value = ft_strndup(start_ref, (input_ref - start_ref));
	flags = compute_qflags_from_raw(value);
	if (!append(token_list, value, TOKEN_WORD, flags))
	{
		free(value);
		free_tok_list(*token_list);
		exit(EXIT_FAILURE);
	}
	free(value);
	prev = get_prev_token(*token_list);
	if (!prev)
		return ;
	if (prev->type == TOKEN_HEREDOC)
	{
		if (prev->next->qflags & (QF_HAS_SQ | QF_HAS_DQ))
			prev->next->qflags |= QF_HD_QUOTED;
	}
}
