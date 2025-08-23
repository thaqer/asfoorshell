/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaassaf <amaassaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:03:34 by tbaniatt          #+#    #+#             */
/*   Updated: 2025/08/22 08:57:06 by amaassaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIROMENT_H
# define ENVIROMENT_H

# include <stdlib.h>

typedef struct s_env
{
	char			*value;
	char			*key; // i add this to store the key separately
	struct s_env	*next;
}	t_env;

t_env				*get_env(char **env);
char				*get_env_value(char *key, t_env *env);
void				set_env_value(char *key, char *value, t_env *env);
char				**env_list_to_array(t_env *env);
void				cleanup_env_node(t_env *node);
int					count_env_with_values(t_env *env);

#endif