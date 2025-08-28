/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_operators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:00:00 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/16 00:00:00 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

int	execute_logical_and(t_ast *ast, t_shell *shell)
{
	int	left_status;

	left_status = execute_ast(ast->s_binary.left, shell);
	if (left_status != 0)
		return (left_status);
	return (execute_ast(ast->s_binary.right, shell));
}

int	execute_logical_or(t_ast *ast, t_shell *shell)
{
	int	left_status;

	left_status = execute_ast(ast->s_binary.left, shell);
	if (left_status == 0)
		return (left_status);
	return (execute_ast(ast->s_binary.right, shell));
}
