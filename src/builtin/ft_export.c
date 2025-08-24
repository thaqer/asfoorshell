/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:06:47 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/24 18:54:13 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*find_env(char *key, t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0
			&& ft_strlen(env->key) == ft_strlen(key))
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	export_identifier_error(char *key, t_shell *shell)
{
	shell->status = 1;
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(key, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static void	export_process(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	int		flag;

	key = NULL;
	value = NULL;
	if (parse_export_arg(arg, &key, &value))
	{
		shell_error_message(strerror(errno));
		return ;
	}
	veiled_key(key, shell, &flag);
	if (set_env(shell, key, value, &flag))
		shell->status = 1;
	if (key)
		free(key);
	if (value)
		free(value);
}

int	ft_export_argv(char **input, t_shell *shell)
{
	int		i;

	if (!input || !input[0])
		return (0);
	i = 0;
	if (input[0] && ft_strncmp(input[0], "export", 7) == 0)
		i = 1;
	if (!input[i])
	{
		print_export_list(shell->env);
		return (0);
	}
	shell->status = 0;
	while (input[i])
	{
		export_process(input[i], shell);
		i++;
	}
	return (shell->status);
}

void	veiled_key(char *key, t_shell *shell, int *flag)
{
	int	i;

	i = 0;
	*flag = 0;
	while (key[i] == ' ')
		i++;
	if (!ft_isalpha(key[0]) && key[0] != '_')
	{
		export_identifier_error(key, shell);
		*flag = 1;
	}
}
