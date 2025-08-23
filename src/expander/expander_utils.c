/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:07:18 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/08/22 09:11:52 by alrfa3i          ###   ########.fr       */
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
	size_t	len;

	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(""));
	return (ft_strndup(str + 1, len - 2));
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
