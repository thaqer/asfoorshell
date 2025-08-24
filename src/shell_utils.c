/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:42:04 by daniah            #+#    #+#             */
/*   Updated: 2025/08/25 00:50:12 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	run_ast_if_no_signal(t_shell *shell)
{
	preprocess_all_heredocs(shell->ast_root, shell);
	if (g_sig_value == 0)
	{
		shell->status = execute_ast(shell->ast_root, shell);
		free_ast(shell->ast_root);
		shell->ast_root = NULL;
	}
}

void	sig_status(t_shell *shell, t_expander_creds *expander_creds)
{
	shell->status = 130;
	expander_creds->last_exit_status = 130;
	g_sig_value = 0;
}
