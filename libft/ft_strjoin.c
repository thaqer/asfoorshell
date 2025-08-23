/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaassaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 20:31:47 by amaassaf          #+#    #+#             */
/*   Updated: 2024/09/03 21:11:41 by amaassaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	size_t	x;
	size_t	y;
	char	*ptr;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = malloc(sizeof(char) * len);
	if (!ptr)
		return (NULL);
	x = 0;
	y = 0;
	while (s1[x] != '\0')
	{
		ptr[x] = s1[x];
		x++;
	}
	while (s2[y] != '\0')
	{
		ptr[x++] = s2[y++];
	}
	ptr[x] = '\0';
	return (ptr);
}
