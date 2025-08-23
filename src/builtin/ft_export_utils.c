/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbaniatt <tbaniatt@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 23:59:00 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/16 00:46:26 by tbaniatt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* create a new env node */
t_env	*new_env_node(const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup((char *)key);
	if (value)
		new->value = ft_strdup((char *)value);
	else
		new->value = NULL;
	if (!new->key || (value && !new->value))
	{
		if (new->key)
			free(new->key);
		if (new->value)
			free(new->value);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

static int	update_existing_env(t_env *node, const char *value, t_shell *shell)
{
	(void)shell;
	if (!value)
		return (0);
	if (node->value)
		free(node->value);
	node->value = ft_strdup((char *)value);
	if (!node->value)
		return (shell_error_message(strerror(errno)));
	return (0);
}

/* set or append an env */
int	set_env(t_shell *shell, const char *key, const char *value)
{
	t_env	*node;
	t_env	*tmp;
	t_env	*new;

	node = find_env((char *)key, shell->env);
	if (node)
		return (update_existing_env(node, value, shell));
	new = new_env_node(key, value);
	if (!new)
		return (shell_error_message(strerror(errno)));
	if (!shell->env)
	{
		shell->env = new;
		return (0);
	}
	tmp = shell->env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (0);
}

/* trim quotes if value surrounded */
void	handle_value(char *value)
{
	size_t	len;

	if (!value)
		return ;
	len = ft_strlen(value);
	if (len >= 2 && value[0] == '"' && value[len - 1] == '"')
	{
		value[len - 1] = '\0';
		ft_memmove(value, value + 1, len - 1);
	}
	len = ft_strlen(value);
	if (len >= 2 && value[0] == '\'' && value[len - 1] == '\'')
	{
		value[len - 1] = '\0';
		ft_memmove(value, value + 1, len - 1);
	}
}

/* parse a single export argument */
int	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal_sign;

	*key = NULL;
	*value = NULL;
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*key = ft_substr(arg, 0, equal_sign - arg);
		*value = ft_strdup(equal_sign + 1);
		if (!*key || !*value)
			return (1);
		handle_value(*value);
	}
	else
	{
		*key = ft_strdup(arg);
		if (!*key)
			return (1);
	}
	return (0);
}
