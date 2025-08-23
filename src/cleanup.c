/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 14:05:01 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/20 21:16:23 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	cleanup_execution(t_execution *execution)
{
	if (execution->saved_stdin != -1)
	{
		close(execution->saved_stdin);
		execution->saved_stdin = -1;
	}
	if (execution->saved_stdout != -1)
	{
		close(execution->saved_stdout);
		execution->saved_stdout = -1;
	}
}

void	cleanup(t_shell *shell)
{
	if (shell->tokens)
	{
		free_tok_list(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if (shell->execution)
	{
		cleanup_execution(shell->execution);
		free(shell->execution);
		shell->execution = NULL;
	}
	if (shell->ast_root)
	{
		free_ast(shell->ast_root);
		shell->ast_root = NULL;
	}
}
// if (shell->parser && shell->parser->errmsg)
// {
// 	// free(shell->parser->errmsg);
// 	shell->parser->errmsg = NULL;
// }

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = NULL;
	}
}

void	ft_free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}
