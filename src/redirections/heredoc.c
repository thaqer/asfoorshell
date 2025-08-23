/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:17:26 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/23 16:50:46 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_sig_value;

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
		if (!line || g_sig_value == SIGINT)
		{
			if (line)
				free(line);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(delimiter))
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	setup_signals(SIGNAL_NONE);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
