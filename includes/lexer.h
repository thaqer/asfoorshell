/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 03:52:32 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/25 12:16:58 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include <stdlib.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	// For bonus
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,
}					t_token_type;

/* Quote/expansion flags for TOKEN_WORD */
typedef enum e_qflags
{
	QF_NONE = 0,
	QF_HAS_SQ = 1 << 0,
	QF_HAS_DQ = 1 << 1,
	QF_ALL_SQ = 1 << 2,
	QF_ALL_DQ = 1 << 3,
	QF_MIXED = 1 << 4, /* mixture: quoted + unquoted, or both quote types */
	QF_HD_QUOTED = 1 << 5
}					t_qflags;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_qflags		qflags;
	struct s_token	*next;
}					t_token;

typedef enum e_separator_type
{
	SEP_NONE = -1,
	SEP_SPACE,
	SEP_PIPE,
	SEP_REDIRECT_IN,
	SEP_REDIRECT_OUT,
	SEP_APPEND,
	SEP_HEREDOC,
	SEP_SQUOTE,
	SEP_DQUOTE,
	//	For Bonus
	SEP_AND,
	SEP_OR,
	SEP_OPEN_PAREN,
	SEP_CLOSE_PAREN,
}					t_separator_type;

typedef enum e_state
{
	STATE_NORMAL,
	STATE_SQUOTE,
	STATE_DQUOTE,
}					t_state;

t_token				*tokenize(char *input);
void				append_separator(t_token **head, t_separator_type type,
						char **input_ref, char **start_ref);
t_token_type		type_determinant(t_separator_type sep_type);
t_separator_type	handle_double_char_sep(char *current_position);
t_separator_type	is_separator(char *current_position);
t_token				*get_prev_token(t_token *head);
void				append_and_handle_failure(t_token **token_list, char *start_ref,
						char *input_ref);
void				normal_state_handler(t_separator_type sep_type, t_token **token_list,
						char **start_ref, char **input_ref);

//                               tok_utils.c
t_token				*create_token(char *value, t_token_type type,
						t_qflags flags);
t_token				*append(t_token **head, char *value, t_token_type type,
						t_qflags flags);
char				*ft_strndup(const char *s, size_t n);
void				free_tok_list(t_token *head);
t_qflags			compute_qflags_from_raw(char *s);
int					insert_at_index(t_token **head, size_t index,
						t_token *new_node);
void				delete_node(t_token **head, const char *value);
void				state_determinant(t_state *state, char **s_ref);

#endif
