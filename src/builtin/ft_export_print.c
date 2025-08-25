/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daniah <daniah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 00:40:00 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/25 05:05:29 by daniah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	insert_sorted(t_env *arr[], int *n, t_env *node)
{
	int	pos;

	pos = *n;
	while (pos > 0 && ft_strncmp(arr[pos - 1]->key, node->key, ft_strlen(arr[pos
					- 1]->key) + 1) > 0)
	{
		arr[pos] = arr[pos - 1];
		pos--;
	}
	arr[pos] = node;
	(*n)++;
}

void	print_export_list(t_env *env)
{
	int		n;
	int		i;
	t_env	*arr[1024];

	n = 0;
	while (env && n < 1024)
	{
		insert_sorted(arr, &n, env);
		env = env->next;
	}
	i = 0;
	while (i < n)
	{
		ft_putstr_fd("export -x ", 1);
		ft_putstr_fd(arr[i]->key, 1);
		if (arr[i]->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(arr[i]->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putstr_fd("\n", 1);
		i++;
	}
}
