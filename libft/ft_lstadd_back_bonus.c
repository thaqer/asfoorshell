/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:55:41 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/27 12:07:45 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*current_last;

	if (lst)
	{
		if (*lst)
		{
			current_last = ft_lstlast(*lst);
			current_last->next = new;
		}
		else
			*lst = new;
	}
}
