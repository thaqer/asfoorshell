/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-08-22 08:35:18 by tbaniatt          #+#    #+#             */
/*   Updated: 2025-08-22 08:35:18 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_signals(int mode)
{
	if (mode == SIGNAL_PARENT)
	{
		signal(SIGINT, handle_signal);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, handle_sigpipe);
	}
	else if (mode == SIGNAL_CHILD)
	{
		signal(SIGINT, handle_signal_child);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);
	}
	else if (mode == SIGNAL_HEREDOC)
	{
		signal(SIGINT, handle_signal_heredoc);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, handle_sigpipe);
	}
	else if (mode == SIGNAL_NONE)
	{
		signal(SIGINT, handle_first);
		signal(SIGQUIT, SIG_IGN);
	}
}
