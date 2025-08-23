/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment_utils_extra.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:00:00 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/16 00:00:00 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_existing_env(t_env *env, char *value)
{
	free(env->value);
	if (value)
		env->value = ft_strdup(value);
	else
		env->value = ft_strdup("");
	if (!env->value)
		env->value = ft_strdup("");
}

static t_env	*create_new_env_node(char *key, char *value)
{
	t_env	*tmp;

	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return (NULL);
	tmp->key = ft_strdup(key);
	if (value)
		tmp->value = ft_strdup(value);
	else
		tmp->value = ft_strdup("");
	if (!tmp->key || !tmp->value)
	{
		cleanup_env_node(tmp);
		return (NULL);
	}
	tmp->next = NULL;
	return (tmp);
}

void	set_env_value(char *key, char *value, t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0
			&& ft_strlen(env->key) == ft_strlen(key))
		{
			update_existing_env(env, value);
			return ;
		}
		if (!env->next)
			break ;
		env = env->next;
	}
	tmp = create_new_env_node(key, value);
	if (tmp)
		env->next = tmp;
}
