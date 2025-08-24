/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:42:05 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/24 18:19:42 by dqaddomi         ###   ########.fr       */
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

int	handle_heredocs_first(t_redir *redirs, t_shell *shell)
{
	t_redir	*current;
	int		last_fd;

	last_fd = -1;
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			if (last_fd != -1)
				close(last_fd);
			if (current->heredoc_fd != -1)
				last_fd = current->heredoc_fd;
			else
				last_fd = handle_heredoc_redir(current->target, shell);
			if (last_fd == -1)
				return (-1);
			if (last_fd == -2)
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
