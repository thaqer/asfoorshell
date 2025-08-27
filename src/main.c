/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 15:29:04 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/27 18:52:25 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static void	cleanup_line(t_shell *shell, t_expander_creds *expander_creds,
		char *line)
{
	if (g_sig_value == SIGINT)
	{
		if (shell->ast_root)
		{
			free_ast(shell->ast_root);
			shell->ast_root = NULL;
		}
		close_unused_fds();
	}
	expander_creds->last_exit_status = shell->status;
	free_tok_list(shell->tokens);
	shell->tokens = NULL;
	free(line);
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
		run_ast_if_no_signal(shell);
	cleanup_line(shell, expander_creds, line);
}

void	exit_shell(t_shell *shell)
{
	cleanup(shell);
	clear_history();
	printf("\rexit\n");
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
			if (g_sig_value != 0)
				sig_status(&shell, &expander_creds);
			process_line(line, &shell, &parser, &expander_creds);
		}
	}
}
