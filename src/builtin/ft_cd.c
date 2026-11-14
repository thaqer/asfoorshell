/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 13:25:45 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/27 19:10:11 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*resolve_cd_path(char **argv, t_shell *shell)
{
	char	*path;

	if (argv[1])
		path = ft_strdup(argv[1]);
	else
		path = get_env_value("HOME", shell->env);
	return (path);
}

static char	*get_current_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	return (pwd);
}

static int	change_directory(char *path, t_shell *shell)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = get_current_pwd();
	if (!oldpwd)
		return (1);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		free(oldpwd);
		return (1);
	}
	pwd = get_current_pwd();
	if (!pwd)
	{
		free(oldpwd);
		return (1);
	}
	end_cd_process(shell, oldpwd, pwd);
	return (0);
}

int	ft_cd_argv(char **argv, t_shell *shell)
{
	char	*path;
	int		result;

	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	path = resolve_cd_path(argv, shell);
	if (!path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	result = change_directory(path, shell);
	free(path);
	return (result);
}

void	end_cd_process(t_shell *shell, char *oldpwd, char *pwd)
{
	set_env_value("OLDPWD", oldpwd, shell->env);
	set_env_value("PWD", pwd, shell->env);
	free(oldpwd);
	free(pwd);
}
