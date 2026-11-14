/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 10:50:11 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/27 12:07:45 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;
	size_t			x;
	unsigned char	ch_search;

	ch_search = (unsigned char)c;
	ptr = (unsigned char *)s;
	x = 0;
	if (n == 0)
		return (NULL);
	while (x < n)
	{
		if (ptr[x] == ch_search)
			return ((void *)&ptr[x]);
		x++;
	}
	return (NULL);
}
