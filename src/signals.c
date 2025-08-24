/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:06:52 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/24 17:32:03 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_sig_value = 0;

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig_value = SIGINT;
	}
}

void	handle_signal_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		close(STDIN_FILENO);
		g_sig_value = SIGINT;
	}
	else if (sig == SIGPIPE)
		g_sig_value = SIGPIPE;
}

void	handle_signal_child(int sig)
{
	g_sig_value = sig;
	if (sig == SIGINT)
		write(STDERR_FILENO, "\n", 1);
}

void	handle_first(int sig)
{
	g_sig_value = sig;
}

void	handle_sigpipe(int sig)
{
	(void)sig;
	g_sig_value = SIGPIPE;
}
