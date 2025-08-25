/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 08:26:40 by amaassaf          #+#    #+#             */
/*   Updated: 2025/08/25 22:14:45 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "redirection.h"

/* ---------- parser state ---------- */
typedef struct s_parser
{
	t_token					*current_token;
	int						error; // 0 = ok, nonzero = syntax error
	char					*errmsg;
}							t_parser;

/*                Main Ast Structures                    */

typedef enum e_ast_type
{
	AST_SIMPLE_COMMAND,
	AST_PIPELINE,
	AST_LOGICAL_AND,
	AST_LOGICAL_OR,
	AST_SUBSHELL
}							t_ast_node_type;

typedef struct s_ast
{
	t_ast_node_type			type;
	union
	{
		struct
		{
			char	**argv;// null-terminated array
			t_redir	*redirs;// linked list of redirections
		}	s_simple_command;
		struct
		{
			struct s_ast	*left;
			struct s_ast	*right;
		}	s_binary; // used for pipeline, logical and/or
		struct
		{
			struct s_ast	*child;
			t_redir			*redirs; // redirections applied to subshell group
		} s_subshell;
	};
}							t_ast;

/* ----------- Core Helpers ------------ */
void						advance(t_parser *parser);
int							match(t_parser *parser, t_token_type type);
t_token						*expect(t_parser *parser, t_token_type type);
void						set_parse_error(t_parser *p, char *msg);

/* ---------- Node Constructors ---------- */
t_ast						*ast_create_simple(char **argv, t_redir *redirs);
t_ast						*ast_create_binary(t_ast_node_type type,
								t_ast *left,
								t_ast *right);
t_ast						*ast_create_subshell(t_ast *child, t_redir *redirs);

/* ---------- free helpers ---------- */
void						free_redirs(t_redir *r);
void						free_argv(char **argv);
void						free_partial_arguments(char **argv, int argc);
void						free_ast(t_ast *n);

/* ---------- clean helpers ---------- */
t_ast						*cleanup_simple_cmd(char **argv, int argc,
								t_redir *redirs);
t_ast						*clean_group(t_ast *inner, t_redir *redirs);

/* ---------- args helpers ---------- */
char						**allocate_argv(t_parser copy);
void						add_args(t_parser *parser, char **argv, int *argc);

/* ---------- redirs helpers ---------- */
int							add_redirection(t_redir **head, t_token_type type,
								const char *target);
int							is_redirection_tok(t_token *token);
t_redir_type				token_to_redir_type(t_token_type type);

/* ---------- parser API ---------- */
int							parse_redirections(t_parser *p, t_redir **list);
t_ast						*parse_simple_command(t_parser *p);
t_ast						*parse_command(t_parser *p);
t_ast						*parse_group(t_parser *parser);
t_ast						*parse_pipeline(t_parser *p);
t_ast						*parse_logical_line(t_parser *p);
t_ast						*parse_input_line(t_parser *parser);

/* parser_helpers.c */
int							handle_parse_logical_error(t_parser *parser,
								t_ast *left);
int							handle_parse_pipeline_error(t_parser *parser,
								t_ast *left);

#endif
