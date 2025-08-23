/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alalauty <alalauty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 00:59:37 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/22 10:37:41 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redir(char *target)
{
	int	fd;

	fd = open(target, O_RDONLY);
	if (fd == -1)
	{
		perror(target);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_output_redir(char *target)
{
	int	fd;

	fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(target);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_append_redir(char *target)
{
	int	fd;

	fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(target);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_single_redir(t_redir *redir, t_shell *shell)
{
	(void)shell;
	if (redir->type == REDIR_IN)
		return (handle_input_redir(redir->target));
	else if (redir->type == REDIR_OUT)
		return (handle_output_redir(redir->target));
	else if (redir->type == REDIR_APPEND)
		return (handle_append_redir(redir->target));
	return (0);
}

int	handle_redirections(t_redir *redirs, t_shell *shell)
{
	t_redir	*current;
	int		result;

	if (handle_heredocs_first(redirs, shell) != 0)
		return (1);
	current = redirs;
	while (current)
	{
		result = handle_single_redir(current, shell);
		if (result != 0)
			return (result);
		current = current->next;
	}
	return (0);
}
