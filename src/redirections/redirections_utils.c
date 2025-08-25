/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:42:05 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/25 05:03:20 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	apply_final_heredoc(int last_fd)
{
	if (last_fd != -1)
	{
		dup2(last_fd, STDIN_FILENO);
		close(last_fd);
	}
}

static int	process_heredoc_redir(t_redir *current, int *last_fd,
		t_shell *shell)
{
	if (*last_fd != -1)
		close(*last_fd);
	if (current->heredoc_fd != -1)
		*last_fd = current->heredoc_fd;
	else
		*last_fd = handle_heredoc_redir(current->target, shell);
	if (*last_fd == -1)
		return (-1);
	if (*last_fd == -2)
		return (-2);
	return (0);
}

int	handle_heredocs_first(t_redir *redirs, t_shell *shell)
{
	t_redir	*current;
	int		last_fd;
	int		result;

	last_fd = -1;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			result = process_heredoc_redir(current, &last_fd, shell);
			if (result == -1)
				return (-1);
			if (result == -2)
				break ;
		}
		current = current->next;
	}
	if (g_sig_value == SIGINT)
	{
		shell->status = 130;
		setup_signals(SIGNAL_NONE);
	}
	apply_final_heredoc(last_fd);
	return (0);
}
