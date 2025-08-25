/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 21:35:00 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/25 21:37:54 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_split_array(char **split, int start_index)
{
	while (split[start_index])
		free(split[start_index++]);
	free(split);
}

int	create_and_insert_token(t_token **head, int *index, char *str)
{
	t_token	*new_token;

	new_token = create_token(str, TOKEN_WORD, QF_EXPANDED);
	if (!new_token)
		return (0);
	if (!insert_at_index(head, *index, new_token))
	{
		free(new_token->value);
		free(new_token);
		return (0);
	}
	(*index)++;
	return (1);
}

int	process_split_strings(t_token **head, int *index, char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		if (!create_and_insert_token(head, index, split[i]))
		{
			cleanup_split_array(split, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	append_split_tokens(t_token **head, int *index, char *str)
{
	char	**split;

	if (!str || !*str)
		return (1);
	split = ft_split(str, ' ');
	if (!split)
		return (0);
	if (!process_split_strings(head, index, split))
		return (0);
	cleanup_split_array(split, 0);
	return (1);
}
