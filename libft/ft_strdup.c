/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaassaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 19:32:56 by amaassaf          #+#    #+#             */
/*   Updated: 2024/09/03 19:39:40 by amaassaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*ptr;
	size_t	n;

	n = ft_strlen(s) + 1;
	ptr = malloc(n);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, s, n);
	return (ptr);
}
/*
char	*ft_strdup(const char *s)
{
	char	*ptr;
	size_t	n;
	size_t	x;

	x = 0;
	n = ft_strlen(s) + 1;
	ptr = malloc(n);
	if (!ptr)
		return (NULL);
	while (s[x])
	{
		ptr[x] = s[x];
		x++;
	}
	ptr[x] = 0;
	return (ptr);
}
*/
