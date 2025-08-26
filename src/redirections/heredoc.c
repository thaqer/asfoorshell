/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:17:26 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/25 04:58:26 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_sig_value;

static int	process_heredoc_line(char *line, char *delimiter, int pipe_fd[2],
		t_shell *shell)
{
	if (!line)
		return (1);
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& ft_strlen(line) == ft_strlen(delimiter))
	{
		free(line);
		return (1);
	}
	if (shell->ast_root->s_subshell.redirs->s_delim_quoted == false)
		line = expand_heredoc_line(line, shell);
	write(pipe_fd[1], line, ft_strlen(line));
	write(pipe_fd[1], "\n", 1);
	free(line);
	return (0);
}

int	handle_heredoc_redir(char *delimiter, t_shell *shell)
{
	char	*line;
	int		pipe_fd[2];

	if (!delimiter || !shell)
		return (-1);
	if (pipe(pipe_fd) == -1)
		return (-1);
	setup_signals(SIGNAL_HEREDOC);
	while (1)
	{
		line = readline("> ");
		if (g_sig_value == SIGINT)
		{
			dup2(2, 0);
			return (-2);
		}
		if (process_heredoc_line(line, delimiter, pipe_fd, shell))
			break ;
	}
	setup_signals(SIGNAL_NONE);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	t_expander_creds	creds;
	char				*expanded_line;

	if (!line || !shell)
		return (NULL);
	creds.last_exit_status = shell->status;
	creds.envp = shell->env;
	creds.program_name = "minishell";
	expanded_line = unquoted_variable_handler(line, &creds);
	if (!expanded_line)
		return (line);
	free(line);
	return (expanded_line);
}
