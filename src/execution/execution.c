/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:58:39 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/25 01:24:20 by daniah           ###   ########.fr       */
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

void	handle_left_pipeline(int *pipefd, t_ast *ast, t_shell *shell)
{
	int	ret;

	setup_signals(SIGNAL_CHILD);
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	ret = execute_ast(ast->s_binary.left, shell);
	cleanup(shell);
	exit(ret);
}

void	handle_right_pipeline(int *pipefd, t_ast *ast, t_shell *shell)
{
	int	ret;

	setup_signals(SIGNAL_CHILD);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	ret = execute_ast(ast->s_binary.right, shell);
	cleanup(shell);
	exit(ret);
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
