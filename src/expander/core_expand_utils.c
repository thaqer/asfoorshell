/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_expand_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 05:20:00 by daniah            #+#    #+#             */
/*   Updated: 2025/08/26 18:44:52 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cleaner(char *str)
{
	free(str);
	return (NULL);
}

char	*append_literal_segment(char *output, char *start, size_t len)
{
	char	*literal_text;
	char	*new_output;

	if (len == 0)
		return (output);
	literal_text = ft_strndup(start, len);
	if (!literal_text)
		return (cleaner(output));
	new_output = ft_strjoin(output, literal_text);
	free(literal_text);
	if (!new_output)
		return (cleaner(output));
	free(output);
	return (new_output);
}

char	*append_string_to_output(char *output, char *str)
{
	char	*new_output;

	new_output = ft_strjoin(output, str);
	if (!new_output)
		return (cleaner(output));
	free(output);
	return (new_output);
}

char	*expand_exit_status(char *output, int status)
{
	char	*status_string;
	char	*new_output;

	status_string = ft_itoa(status);
	if (!status_string)
		return (cleaner(output));
	new_output = ft_strjoin(output, status_string);
	free(status_string);
	if (!new_output)
		return (cleaner(output));
	free(output);
	return (new_output);
}

char	*extract_variable_name(char *start, char **end_pos)
{
	char		*current;
	t_params	char_type;

	current = start;
	while (*current)
	{
		char_type = param_determinant(*current);
		if (!(char_type == ALPHA || char_type == UNDERSCORE
				|| char_type == NUMERIC))
			break ;
		current++;
	}
	*end_pos = current;
	return (ft_strndup(start, (size_t)(current - start)));
}
