/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:58:39 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/24 20:45:11 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_external_command(char **argv, t_shell *shell)
{
	char	*command_path;
	int		status;

	command_path = find_command_in_path(argv[0], shell);
	if (!command_path)
	{
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	status = fork_and_execute(command_path, argv, shell);
	free(command_path);
	return (status);
}

char	*find_command_in_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*path_env;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value("PATH", shell->env);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	free(path_env);
	return (search_in_path_dirs(paths, cmd));
}

int	execute_pipeline(t_ast *ast, t_shell *shell)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	left_pid = fork();
	if (left_pid == 0)
		handle_left_pipeline(pipefd, ast, shell);
	right_pid = fork();
	if (right_pid == 0)
		handle_right_pipeline(pipefd, ast, shell);
	close(pipefd[0]);
	close(pipefd[1]);
	setup_signals(SIGNAL_NONE);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	cleanup_heredoc_fds(ast);
	return (WEXITSTATUS(status));
}

static t_redir	*process_heredoc_list(t_redir *redirs, t_shell *shell)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			current->heredoc_fd = handle_heredoc_redir(current->target,
					shell);
			if (current->heredoc_fd == -2)
				break ;
		}
		current = current->next;
	}
	return (current);
}

void	preprocess_all_heredocs(t_ast *ast, t_shell *shell)
{
	t_redir	*current;

	if (!ast)
		return ;
	if (ast->type == AST_SIMPLE_COMMAND && ast->s_simple_command.redirs)
		current = process_heredoc_list(ast->s_simple_command.redirs, shell);
	else if (ast->type == AST_PIPELINE || ast->type == AST_LOGICAL_AND
		|| ast->type == AST_LOGICAL_OR)
	{
		preprocess_all_heredocs(ast->s_binary.left, shell);
		preprocess_all_heredocs(ast->s_binary.right, shell);
	}
	else if (ast->type == AST_SUBSHELL && ast->s_subshell.redirs)
	{
		current = process_heredoc_list(ast->s_subshell.redirs, shell);
		if (current && current->heredoc_fd != -2)
			preprocess_all_heredocs(ast->s_subshell.child, shell);
	}
	if (g_sig_value == SIGINT)
	{
		if (current)
			current->heredoc_fd = -1;
		shell->status = 130;
		setup_signals(SIGNAL_NONE);
	}
}
