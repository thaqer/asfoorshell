/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:06:47 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/16 00:24:35 by tbaniatt         ###   ########.fr       */
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

	key = NULL;
	value = NULL;
	if (parse_export_arg(arg, &key, &value))
	{
		shell_error_message(strerror(errno));
		return ;
	}
	veiled_key(key, shell);
	if (shell->status == 1)
	{
		if (key)
			free(key);
		if (value)
			free(value);
		return ;
	}
	if (set_env(shell, key, value))
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

void	veiled_key(char *key, t_shell *shell)
{
	int	i;

	i = 0;
	while (key[i] == ' ')
		i++;
	if (key[0] == '_' || ft_isalpha(key[0]))
	{
		while (key[i] && key[i] != ' ' && key[i] != '=')
		{
			if (!ft_isalnum(key[i]) && key[i] != '_')
			{
				export_identifier_error(key, shell);
				return ;
			}
			i++;
		}
	}
	else
		export_identifier_error(key, shell);
}
