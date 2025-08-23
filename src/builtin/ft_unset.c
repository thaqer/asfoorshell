/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 01:12:31 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/13 21:33:52 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0] || ft_isdigit(str[0]))
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	env_key_matches(char *env_key, char *key, size_t key_len)
{
	if (!env_key || !key)
		return (0);
	if (ft_strncmp(env_key, key, key_len) == 0
		&& ft_strlen(env_key) == key_len)
		return (1);
	return (0);
}

static void	remove_env_node(t_env *node, t_env *prev, t_shell *shell)
{
	if (prev)
		prev->next = node->next;
	else
		shell->env = node->next;
	free(node->key);
	free(node->value);
	free(node);
}

static int	unset_single_var(char *key, t_shell *shell)
{
	t_env	*tmp;
	t_env	*prev;
	size_t	key_len;

	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: unset: `", 2);
		ft_putstr_fd(key, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	key_len = ft_strlen(key);
	tmp = shell->env;
	prev = NULL;
	while (tmp)
	{
		if (env_key_matches(tmp->key, key, key_len))
		{
			remove_env_node(tmp, prev, shell);
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (0);
}

int	ft_unset_argv(char **argv, t_shell *shell)
{
	int	i;
	int	exit_status;

	if (!argv || !shell)
		return (1);
	if (!argv[1])
		return (1);
	exit_status = 0;
	i = 1;
	while (argv[i])
	{
		if (unset_single_var(argv[i], shell) != 0)
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
