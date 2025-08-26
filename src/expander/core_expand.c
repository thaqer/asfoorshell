/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 08:41:03 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/26 20:03:46 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_numeric_value(char *output, char **pos, char *program_name)
{
	size_t	len;

	len = ft_strlen(program_name) - 2;
	program_name += 2;
	if (*((*pos)++) == '0')
		return (append_literal_segment(output, program_name, len));
	else
		return (output);
}

char	*get_env_var_value(char *var_start, t_env *envp, char **end_pos)
{
	char	*variable_name;
	char	*variable_value;

	variable_name = extract_variable_name(var_start, end_pos);
	if (!variable_name)
		return (NULL);
	variable_value = get_env_value(variable_name, envp);
	free(variable_name);
	if (!variable_value)
	{
		variable_value = ft_strdup("");
		if (!variable_value)
			return (NULL);
	}
	return (variable_value);
}

char	*expand_env_variable(char *output, char *var_start, t_env *envp,
		char **end_pos)
{
	char	*variable_value;
	char	*new_output;

	variable_value = get_env_var_value(var_start, envp, end_pos);
	if (!variable_value)
		return (cleaner(output));
	new_output = ft_strjoin(output, variable_value);
	free(variable_value);
	if (!new_output)
		return (cleaner(output));
	free(output);
	return (new_output);
}

char	*init_expander(char *input_string, char **unquoted_str)
{
	char	*expanded_output;

	*unquoted_str = strip_all_outer_quotes(input_string);
	if (!*unquoted_str)
		return (NULL);
	expanded_output = ft_strdup("");
	if (!expanded_output)
	{
		free(*unquoted_str);
		return (NULL);
	}
	return (expanded_output);
}
