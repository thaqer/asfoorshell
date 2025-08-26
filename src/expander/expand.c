/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:00:09 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/26 08:42:06 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_argv_size(char **argv)
{
	size_t	size;

	size = 0;
	while (argv[size])
		size++;
	return (size);
}

int	splice_unquoted_expansion(t_token **head, int index, char *str)
{
	char	**split;
	int		i;
	t_token	*new_token;
	int		j;

	split = ft_split(str, ' ');
	i = 0;
	if (!split)
		return (0);
	while (split[i])
	{
		new_token = create_token(split[i], TOKEN_WORD, QF_NONE);
		if (!new_token)
			return (0);
		insert_at_index(head, index, new_token);
		i++;
		index++;
	}
	j = 0;
	while (split[j])
		free(split[j++]);
	free(split);
	if (i >= 1)
		delete_node(head, str);
	return (1);
}

static int	handle_quoted_tokens(t_token *cur, t_expander_creds *expander_creds)
{
	char	*old_value;

	if (word_all_single(cur->qflags))
	{
		old_value = cur->value;
		cur->value = strip_quotes(cur->value);
		free(old_value);
		if (!word_hd_quote(cur->qflags))
			cur->qflags = QF_EXPANDED;
	}
	else if (word_all_double(cur->qflags))
	{
		old_value = cur->value;
		cur->value = dquote_expander(cur->value, expander_creds);
		free(old_value);
		if (!cur->value)
			return (0);
		if (!word_hd_quote(cur->qflags))
			cur->qflags = QF_EXPANDED;
	}
	return (1);
}

static int	process_token_expansion(t_token *cur, t_token **head, size_t index,
		t_expander_creds *expander_creds)
{
	char	*old_value;

	if (cur->qflags & QF_EXPANDED)
		return (1);
	if (word_all_single(cur->qflags) || word_all_double(cur->qflags))
		return (handle_quoted_tokens(cur, expander_creds));
	if (word_unquoted(cur->qflags))
	{
		old_value = cur->value;
		cur->value = unquoted_variable_handler(cur->value, expander_creds);
		free(old_value);
		if (!cur->value)
			return (0);
		if (ft_strchr(cur->value, ' '))
		{
			if (!splice_unquoted_expansion(head, index, cur->value))
				return (0);
		}
	}
	else if (word_mixed(cur->qflags))
	{
		if (!splice_mixed_expansion(head, index, cur->value, expander_creds))
			return (0);
		return (2);
	}
	return (1);
}

int	expand_tokens(t_token **head, t_expander_creds *expander_creds)
{
	t_token	*cur;
	size_t	index;
	int		result;

	cur = *head;
	index = 0;
	while (cur)
	{
		if (cur->type == TOKEN_WORD)
		{
			result = process_token_expansion(cur, head, index, expander_creds);
			if (result == 0)
				return (0);
			if (result == 2)
			{
				cur = *head;
				index = 0;
				continue ;
			}
		}
		cur = cur->next;
		index++;
	}
	return (1);
}
