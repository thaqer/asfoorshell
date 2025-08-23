/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:01:15 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/19 20:38:58 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_env_node_values(t_env *tmp, char *env_str, char *equal_pos)
{
	if (equal_pos)
	{
		tmp->key = ft_substr(env_str, 0, equal_pos - env_str);
		tmp->value = ft_strdup(equal_pos + 1);
	}
	else
	{
		tmp->key = ft_strdup(env_str);
		tmp->value = ft_strdup("");
	}
}

static t_env	*create_env_from_string(char *env_str)
{
	t_env	*tmp;
	char	*equal_pos;

	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return (NULL);
	equal_pos = ft_strchr(env_str, '=');
	init_env_node_values(tmp, env_str, equal_pos);
	if (!tmp->key || !tmp->value)
	{
		cleanup_env_node(tmp);
		return (NULL);
	}
	tmp->next = 0;
	return (tmp);
}

t_env	*get_env(char **env)
{
	t_env	*env_list;
	t_env	*tmp;
	int		i;

	i = 0;
	env_list = NULL;
	while (env[i])
	{
		tmp = create_env_from_string(env[i]);
		if (!tmp)
			return (NULL);
		if (!env_list)
			env_list = tmp;
		else
		{
			tmp->next = env_list;
			env_list = tmp;
		}
		i++;
	}
	return (env_list);
}

char	*get_env_value(char *key, t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0
			&& ft_strlen(env->key) == ft_strlen(key))
		{
			return (ft_strdup(env->value));
		}
		env = env->next;
	}
	return (NULL);
}
