/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaassaf <amaassaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:17:30 by amaassaf          #+#    #+#             */
/*   Updated: 2024/09/15 12:47:27 by amaassaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*ptr1;
	unsigned char	*ptr2;
	size_t			x;

	ptr1 = (unsigned char *)dest;
	ptr2 = (unsigned char *)src;
	x = 0;
	if (!dest && !src)
		return (NULL);
	while (x < n)
	{
		ptr1[x] = ptr2[x];
		x++;
	}
	return (dest);
}
