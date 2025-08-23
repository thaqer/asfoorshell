/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher_expander.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:05:14 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/08/22 09:06:31 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*dquote_expander(char *input_string, t_expander_creds *expander_creds)
{
	char	*unquoted_string;
	char	*expanded_output;

	expanded_output = init_expander(input_string, &unquoted_string);
	if (!expanded_output)
		return (NULL);
	expanded_output = expansion_loop(expanded_output,
			unquoted_string, expander_creds);
	if (!expanded_output)
		return (NULL);
	free(unquoted_string);
	return (expanded_output);
}

char	*unquoted_variable_handler(char *input_string,
	t_expander_creds *expander_creds)
{
	char	*expanded_output;

	expanded_output = ft_strdup("");
	if (!expanded_output)
		return (NULL);
	expanded_output = expansion_loop(expanded_output,
			input_string, expander_creds);
	if (!expanded_output)
		return (NULL);
	return (expanded_output);
}
