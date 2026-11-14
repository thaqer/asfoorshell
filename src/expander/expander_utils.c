/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:07:18 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/08/27 17:58:15 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	word_unquoted(unsigned int f)
{
	return (f == QF_NONE);
}

bool	allow_param_expansion(unsigned int f)
{
	return (!word_all_single(f));
}

bool	allow_glob(unsigned int f)
{
	return (!word_has_quotes(f));
}

char	*strip_quotes(char *str)
{
	size_t	i;
	size_t	j;
	char	*out;
	char	quote;

	i = 0;
	j = 0;
	out = malloc(ft_strlen(str) + 1);
	if (!out)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				out[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else
			out[j++] = str[i++];
	}
	out[j] = '\0';
	return (out);
}

t_params	param_determinant(char c)
{
	t_params	params;

	params = OTHER;
	if (c == '_')
		params = UNDERSCORE;
	else if (ft_isalpha(c))
		params = ALPHA;
	else if (ft_isdigit(c))
		params = NUMERIC;
	else if (c == '?')
		params = Q_MARK;
	return (params);
}
