/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:00:00 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/26 17:29:32 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*search_in_path_dirs(char **paths, char *cmd)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		temp = ft_strjoin(full_path, cmd);
		free(full_path);
		if (access(temp, X_OK) == 0)
		{
			ft_free_array(paths);
			return (temp);
		}
		free(temp);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}

static void	execute_child_process(char *command_path, char **argv,
		t_shell *shell)
{
	char	**env_array;

	setup_signals(SIGNAL_CHILD);
	close_unused_fds();
	env_array = env_list_to_array(shell->env);
	execve(command_path, argv, env_array);
	perror("execve");
	if (env_array)
		ft_free_array(env_array);
	if (errno == EACCES)
	{
		free(command_path);
		cleanup(shell);
		exit(126);
	}
	exit(127);
}

int	fork_and_execute(char *command_path, char **argv, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		execute_child_process(command_path, argv, shell);
	if (pid > 0)
	{
		setup_signals(SIGNAL_NONE);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	return (1);
}

static void	cleanup_redirection_heredocs(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_HEREDOC && current->heredoc_fd != -1)
		{
			close(current->heredoc_fd);
			current->heredoc_fd = -1;
		}
		current = current->next;
	}
}

void	cleanup_heredoc_fds(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == AST_SIMPLE_COMMAND && ast->s_simple_command.redirs)
		cleanup_redirection_heredocs(ast->s_simple_command.redirs);
	else if (ast->type == AST_PIPELINE || ast->type == AST_LOGICAL_AND
		|| ast->type == AST_LOGICAL_OR)
	{
		cleanup_heredoc_fds(ast->s_binary.left);
		cleanup_heredoc_fds(ast->s_binary.right);
	}
	else if (ast->type == AST_SUBSHELL && ast->s_subshell.redirs)
		cleanup_redirection_heredocs(ast->s_subshell.redirs);
}
