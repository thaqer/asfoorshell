/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 15:29:04 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/23 17:07:09 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

const char	*token_type_to_string(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	else if (type == TOKEN_PIPE)
		return ("PIPE");
	else if (type == TOKEN_REDIRECT_IN)
		return ("REDIRECT_IN");
	else if (type == TOKEN_REDIRECT_OUT)
		return ("REDIRECT_OUT");
	else if (type == TOKEN_APPEND)
		return ("APPEND");
	else if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	else if (type == TOKEN_AND)
		return ("AND");
	else if (type == TOKEN_OR)
		return ("OR");
	else if (type == TOKEN_OPEN_PAREN)
		return ("OPEN_PAREN");
	else if (type == TOKEN_CLOSE_PAREN)
		return ("CLOSE_PAREN");
	else
		return ("UNKNOWN");
}

int	initialize_shell(t_shell *shell, t_expander_creds *expander_creds,
		char **argv, char **environment)
{
	ft_bzero(shell, sizeof(*shell));
	ft_bzero(expander_creds, sizeof(*expander_creds));
	expander_creds->program_name = argv[0];
	expander_creds->last_exit_status = 0;
	shell->execution = ft_calloc(1, sizeof(*shell->execution));
	if (!shell->execution)
	{
		perror("calloc");
		return (1);
	}
	shell->execution->saved_stdin = -1;
	shell->execution->saved_stdout = -1;
	shell->env = get_env(environment);
	if (!shell->env)
	{
		write(2, "minishell: Failed to initialize environment\n", 42);
		return (1);
	}
	return (0);
}

void	process_line(char *line, t_shell *shell, t_parser *parser,
		t_expander_creds *expander_creds)
{
	if (line && *line)
		add_history(line);
	shell->tokens = tokenize(line);
	if (!shell->tokens)
	{
		free(line);
		return ;
	}
	expander_creds->envp = shell->env;
	expand_tokens(&shell->tokens, expander_creds);
	ft_bzero(parser, sizeof(*parser));
	shell->parser->current_token = shell->tokens;
	shell->ast_root = parse_input_line(shell->parser);
	if (shell->parser->error)
		shell->status = 2;
	else
	{
		preprocess_all_heredocs(shell->ast_root, shell);
		shell->status = execute_ast(shell->ast_root, shell);
		free_ast(shell->ast_root);
		shell->ast_root = NULL;
	}
	expander_creds->last_exit_status = shell->status;
	free_tok_list(shell->tokens);
	shell->tokens = NULL;
	free(line);
}

void	exit_shell(t_shell *shell)
{
	cleanup(shell);
	clear_history();
	rl_free_line_state();
	exit(0);
}

int	main(int argc, char **argv, char **environment)
{
	t_shell				shell;
	char				*line;
	t_parser			parser;
	t_expander_creds	expander_creds;

	if (initialize_shell(&shell, &expander_creds, argv, environment))
		exit(EXIT_FAILURE);
	shell.parser = &parser;
	(void)argc;
	(void)argv;
	(void)environment;
	if (isatty(STDIN_FILENO))
	{
		while (1)
		{
			setup_signals(SIGNAL_PARENT);
			line = readline("\033[93masfoorshell🐥\033[0m:~ ");
			if (line == NULL)
				exit_shell(&shell);
			process_line(line, &shell, &parser, &expander_creds);
		}
	}
	else
	{
		/* Non-interactive mode - read from stdin */
		size_t len = 0;
		ssize_t read_result;

		while ((read_result = getline(&line, &len, stdin)) != -1)
		{
			/* Remove trailing newline if present */
			if (read_result > 0 && line[read_result - 1] == '\n')
				line[read_result - 1] = '\0';
			process_line(line, &shell, &parser, &expander_creds);
		}
	}
	
}
