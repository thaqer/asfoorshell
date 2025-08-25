/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_expand_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 05:25:00 by daniah            #+#    #+#             */
/*   Updated: 2025/08/25 05:27:23 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_dollar_expansion(char *output, char **current_pos,
		t_expander_creds *expander_creds)
{
	char		*next_char;
	char		*result;
	t_params	next_char_type;

	next_char = *current_pos + 1;
	if (*next_char == '\0')
	{
		*current_pos = next_char;
		return (append_string_to_output(output, "$"));
	}
	next_char_type = param_determinant(*next_char);
	if (next_char_type != ALPHA && next_char_type != UNDERSCORE)
		*current_pos = next_char;
	result = handle_dollar_type(output, next_char_type, &next_char,
			expander_creds);
	*current_pos = next_char;
	return (result);
}

char	*process_literal_segment(char *output, char *start, char *current,
		char *unquoted_str)
{
	if (current > start)
	{
		output = append_literal_segment(output, start, (size_t)(current
					- start));
		if (!output)
		{
			free(unquoted_str);
			return (NULL);
		}
	}
	return (output);
}

char	*process_remaining_literal(char *output, char *start, char *current,
		char *unquoted_str)
{
	if (current > start)
	{
		output = append_literal_segment(output, start, (size_t)(current
					- start));
		if (!output)
		{
			free(unquoted_str);
			return (NULL);
		}
	}
	return (output);
}

char	*expansion_loop(char *output, char *unquoted_str,
		t_expander_creds *expander_creds)
{
	char	*current_pos;
	char	*segment_start;

	current_pos = unquoted_str;
	segment_start = current_pos;
	while (*current_pos)
	{
		if (*current_pos != '$')
		{
			current_pos++;
			continue ;
		}
		output = process_literal_segment(output, segment_start, current_pos,
				unquoted_str);
		if (!output)
			return (NULL);
		output = process_dollar_expansion(output, &current_pos, expander_creds);
		if (!output)
			return (cleaner(unquoted_str));
		segment_start = current_pos;
	}
	return (process_remaining_literal(output, segment_start, current_pos,
			unquoted_str));
}

char	*handle_dollar_type(char *output, t_params type, char **pos,
		t_expander_creds *expander_creds)
{
	if (type == Q_MARK)
	{
		*pos = *pos + 1;
		return (expand_exit_status(output, expander_creds->last_exit_status));
	}
	if (type == NUMERIC)
	{
		return (expand_numeric_value(output, pos,
				expander_creds->program_name));
	}
	if (type == ALPHA || type == UNDERSCORE)
		return (expand_env_variable(output, *pos, expander_creds->envp, pos));
	return (append_string_to_output(output, "$"));
}
