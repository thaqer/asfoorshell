/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:00:00 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/16 00:00:00 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_env_node(t_env *node)
{
	if (!node)
		return ;
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

int	count_env_with_values(t_env *env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		if (tmp->value)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

static int	fill_env_array(t_env *env, char **env_array)
{
	int		i;
	char	*temp_str;

	i = 0;
	while (env)
	{
		if (env->value)
		{
			temp_str = ft_strjoin(env->key, "=");
			if (!temp_str)
				return (0);
			env_array[i] = ft_strjoin(temp_str, env->value);
			free(temp_str);
			if (!env_array[i])
				return (0);
			i++;
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (1);
}

char	**env_list_to_array(t_env *env)
{
	char	**env_array;
	int		count;

	if (!env)
		return (NULL);
	count = count_env_with_values(env);
	if (count == 0)
		return (NULL);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	if (!fill_env_array(env, env_array))
		return (ft_free_array(env_array), NULL);
	return (env_array);
}
