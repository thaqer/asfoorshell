/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 08:16:38 by amaassaf          #+#    #+#             */
/*   Updated: 2025/08/25 05:27:23 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "enviroment.h"
# include "lexer.h"
# include "minishell.h"
# include "parser.h"
# include <stdbool.h>

typedef enum e_params
{
	UNDERSCORE,
	ALPHA,
	NUMERIC,
	Q_MARK,
	OTHER,
}						t_params;

typedef struct s_expander_creds
{
	int					last_exit_status;
	t_env				*envp;
	char				*program_name;
}						t_expander_creds;

typedef enum e_segment_type
{
	SEGMENT_UNQUOTED,
	SEGMENT_SINGLE_QUOTED,
	SEGMENT_DOUBLE_QUOTED
}						t_segment_type;

typedef struct s_segment
{
	char				*value;
	t_segment_type		type;
	struct s_segment	*next;
}						t_segment;

bool					word_all_single(unsigned int f);
bool					word_all_double(unsigned int f);
bool					word_has_quotes(unsigned int f);
bool					word_mixed(unsigned int f);
bool					word_unquoted(unsigned int f);
bool					allow_param_expansion(unsigned int f);
bool					allow_glob(unsigned int f);
bool					word_hd_quote(unsigned int f);
t_params				param_determinant(char c);
char					*strip_quotes(char *str);
t_params				param_determinant(char c);
int						expand_tokens(t_token **head,
							t_expander_creds *expander_creds);
char					*cleaner(char *str);
char					*append_literal_segment(char *output,
							char *start, size_t len);
char					*append_string_to_output(char *output, char *str);
char					*expand_exit_status(char *output, int status);
char					*extract_variable_name(char *start, char **end_pos);
char					*expand_numeric_value(char *output,
							char **pos, char *program_name);
char					*init_expander(char *input_string, char **unquoted_str);
char					*process_dollar_expansion(char *output,
							char **current_pos,
							t_expander_creds *expander_creds);
char					*process_literal_segment(char *output, char *start,
							char *current, char *unquoted_str);
char					*process_remaining_literal(char *output,
							char *start, char *current, char *unquoted_str);
char					*expansion_loop(char *output,
							char *unquoted_str,
							t_expander_creds *expander_creds);
char					*handle_dollar_type(char *output,
							t_params type, char **pos,
							t_expander_creds *expander_creds);
char					*get_env_var_value(char *var_start, t_env *envp,
							char **end_pos);
char					*expand_env_variable(char *output, char *var_start,
							t_env *envp, char **end_pos);
char					*expand_numeric_value(char *output,
							char **pos, char *program_name);
char					*handle_dollar_type(char *output,
							t_params type, char **pos,
							t_expander_creds *expander_creds);
char					*process_dollar_expansion(char *output,
							char **current_pos,
							t_expander_creds *expander_creds);
char					*init_expander(char *input_string, char **unquoted_str);
char					*process_remaining_literal(char *output,
							char *start, char *current, char *unquoted_str);
char					*expansion_loop(char *output,
							char *unquoted_str,
							t_expander_creds *expander_creds);
char					*dquote_expander(char *input_string,
							t_expander_creds *expander_creds);
char					*unquoted_variable_handler(char *input_string,
							t_expander_creds *expander_creds);
int						splice_mixed_expansion(t_token **head, int index,
							char *input, t_expander_creds *expander_creds);

#endif
