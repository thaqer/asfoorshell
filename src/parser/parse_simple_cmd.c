/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:47:44 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/26 14:43:36 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_argv_size(t_parser parser)
{
	size_t	size;

	size = 0;
	while (parser.current_token)
	{
		if (parser.current_token->type == TOKEN_WORD)
		{
			++size;
			advance(&parser);
		}
		else if (is_redirection_tok(parser.current_token))
		{
			advance(&parser);
			if (match(&parser, TOKEN_WORD))
				advance(&parser);
		}
		else
			break ;
	}
	return (size);
}

char	**allocate_argv(t_parser parser)
{
	size_t	size;
	char	**args;
	t_token	*token;

	args = NULL;
	token = parser.current_token;
	size = count_argv_size(parser);
	if (!size)
	{
		if (is_redirection_tok(token))
			args = ft_calloc(2, sizeof(char *));
	}
	else
		args = ft_calloc(size + 1, sizeof(char *));
	return (args);
}

void	add_args(t_parser *parser, char **argv, int *argc)
{
	argv[*argc] = ft_strdup(parser->current_token->value);
	if (!argv[*argc])
	{
		set_parse_error(parser, "allocation error");
		return ;
	}
	(*argc)++;
	advance(parser);
}

t_ast	*cleanup_simple_cmd(char **argv, int argc, t_redir *redirs)
{
	free_partial_arguments(argv, argc);
	free_redirs(redirs);
	return (NULL);
}

t_ast	*parse_simple_command(t_parser *parser)
{
	char	**argv;
	int		argc;
	t_redir	*redirs;

	redirs = NULL;
	argc = 0;
	argv = allocate_argv(*parser);
	if (!argv)
		return (NULL);
	while (!parser->error && parser->current_token)
	{
		if (match(parser, TOKEN_WORD))
			add_args(parser, argv, &argc);
		else if (is_redirection_tok(parser->current_token))
		{
			if (parse_redirections(parser, &redirs) < 0)
				return (cleanup_simple_cmd(argv, argc, redirs));
		}
		else
			break ;
	}
	if (parser->error)
		return (cleanup_simple_cmd(argv, argc, redirs));
	argv[argc] = NULL;
	return (ast_create_simple(argv, redirs));
}
