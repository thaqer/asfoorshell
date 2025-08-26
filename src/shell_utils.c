/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:42:04 by daniah            #+#    #+#             */
/*   Updated: 2025/08/26 17:30:05 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

const char	*token_type_to_string(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	else if (type == TOKEN_PIPE)
		return ("PIPE");
	else if (type == TOKEN_REDIRECT_IN)
		return ("REDIRECT_IN");
	else if (type == TOKEN_REDIRECT_OUT)
		return ("REDIRECT_OUT");
	else if (type == TOKEN_APPEND)
		return ("APPEND");
	else if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	else if (type == TOKEN_AND)
		return ("AND");
	else if (type == TOKEN_OR)
		return ("OR");
	else if (type == TOKEN_OPEN_PAREN)
		return ("OPEN_PAREN");
	else if (type == TOKEN_CLOSE_PAREN)
		return ("CLOSE_PAREN");
	else
		return ("UNKNOWN");
}

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
