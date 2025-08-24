/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:00:09 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/08/24 12:54:44 by dqaddomi         ###   ########.fr       */
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
	{
		j = 0;
		while (split[j])
			free(split[j++]);
		free(split);
	}
	if (i >= 1)
		delete_node(head, str);
	return (1);
}

int	expand_tokens(t_token **head, t_expander_creds *expander_creds)
{
	t_token	*cur;
	size_t	index;
	char	*old_value;

	cur = *head;
	index = 0;
	while (cur)
	{
		if (cur->type == TOKEN_WORD)
		{
			if (word_all_single(cur->qflags))
			{
				old_value = cur->value;
				cur->value = strip_quotes(cur->value);
				free(old_value);
				if (!word_hd_quote(cur->qflags))
					cur->qflags = QF_NONE;
			}
			else if (word_all_double(cur->qflags))
			{
				old_value = cur->value;
				cur->value = dquote_expander(cur->value, expander_creds);
				free(old_value);
				if (!cur->value)
					return (0);
				if (!word_hd_quote(cur->qflags))
					cur->qflags = QF_NONE;
			}
			else if (word_unquoted(cur->qflags))
			{
				old_value = cur->value;
				cur->value = unquoted_variable_handler(cur->value,
						expander_creds);
				free(old_value);
				if (!cur->value)
					return (0);
				// Only splice if the expanded value contains spaces (indicating splitting is needed)
				if (ft_strchr(cur->value, ' '))
				{
					if (!splice_unquoted_expansion(head, index, cur->value))
						return (0);
				}
			}
			else if (word_mixed(cur->qflags))
			{
				if (!splice_mixed_expansion(head, index, cur->value,
						expander_creds))
					return (0);
				cur = *head;
					// I fixed invalid read for "l"s (Restart from *head after splice)Because splice changes the list
				index = 0;
				continue ;
			}
		}
		cur = cur->next;
		index++;
	}
	return (1);
}
