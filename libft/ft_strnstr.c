/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaassaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 13:05:09 by amaassaf          #+#    #+#             */
/*   Updated: 2024/09/02 13:46:59 by amaassaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	x;
	size_t	y;

	x = 0;
	y = 0;
	if (*needle == '\0')
		return ((char *)haystack);
	while (haystack[x] && x < len)
	{
		while (needle[y] && haystack[x + y] == needle[y] && haystack[x + y] && x
			+ y < len)
			y++;
		if (needle[y] == '\0')
			return ((char *)&haystack[x]);
		x++;
		y = 0;
	}
	return (NULL);
}
