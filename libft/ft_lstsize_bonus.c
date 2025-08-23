/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaassaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:47:09 by amaassaf          #+#    #+#             */
/*   Updated: 2024/09/15 12:00:48 by amaassaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	size_t	x;
	t_list	*current;

	x = 0;
	current = lst;
	while (current)
	{
		current = current->next;
		x++;
	}
	return (x);
}
