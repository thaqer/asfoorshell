/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 14:10:36 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/20 21:08:11 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin_command(char *cmd)
{
	t_builtin	builtins[8];
	int			i;

	builtins[0] = (t_builtin){"echo", ft_echo_argv};
	builtins[1] = (t_builtin){"cd", ft_cd_argv};
	builtins[2] = (t_builtin){"pwd", ft_pwd};
	builtins[3] = (t_builtin){"export", ft_export_argv};
	builtins[4] = (t_builtin){"unset", ft_unset_argv};
	builtins[5] = (t_builtin){"env", print_env};
	builtins[6] = (t_builtin){"exit", ft_exit_argv};
	builtins[7] = (t_builtin){NULL, NULL};
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strncmp(cmd, builtins[i].name, ft_strlen(builtins[i].name)) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	execute_builtin(char **argv, t_shell *shell)
{
	t_builtin	builtins[8];
	int			i;

	builtins[0] = (t_builtin){"echo", ft_echo_argv};
	builtins[1] = (t_builtin){"cd", ft_cd_argv};
	builtins[2] = (t_builtin){"pwd", ft_pwd};
	builtins[3] = (t_builtin){"export", ft_export_argv};
	builtins[4] = (t_builtin){"unset", ft_unset_argv};
	builtins[5] = (t_builtin){"env", print_env};
	builtins[6] = (t_builtin){"exit", ft_exit_argv};
	builtins[7] = (t_builtin){NULL, NULL};
	i = 0;
	while (builtins[i].name)
	{
		if (ft_strncmp(argv[0], builtins[i].name,
				ft_strlen(builtins[i].name)) == 0)
		{
			return (builtins[i].func(argv, shell));
		}
		i++;
	}
	return (1);
}

int	print_env(char **args, t_shell *shell)
{
	t_env	*env;

	(void)args;
	env = shell->env;
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\n", 1);
		}
		env = env->next;
	}
	return (0);
}
