/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 08:41:03 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/08/22 08:59:17 by alrfa3i          ###   ########.fr       */
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

char	*get_env_var_value( char *var_start, t_env *envp,
								char **end_pos)
{
	char		*variable_name;
	char		*variable_value;
	t_params	type;

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
	type = param_determinant(**end_pos);
	if (type == OTHER && **end_pos != '\0' && **end_pos != '\'')
		(*end_pos)++;
	return (variable_value);
}

char	*expand_env_variable(char *output, char *var_start,
								t_env *envp, char **end_pos)
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

char	*handle_dollar_type(char *output, t_params type,
	char **pos, t_expander_creds *expander_creds)
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
	result = handle_dollar_type(output, next_char_type,
			&next_char, expander_creds);
	*current_pos = next_char;
	return (result);
}

char	*init_expander(char *input_string, char **unquoted_str)
{
	char	*expanded_output;

	*unquoted_str = strip_quotes(input_string);
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

char	*process_literal_segment(char *output, char *start, char *current,
									char *unquoted_str)
{
	if (current > start)
	{
		output = append_literal_segment(output, start,
				(size_t)(current - start));
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
		output = append_literal_segment(output, start,
				(size_t)(current - start));
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
		output = process_literal_segment(output, segment_start,
				current_pos, unquoted_str);
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
