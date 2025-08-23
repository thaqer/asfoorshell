/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaassaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 17:10:16 by amaassaf          #+#    #+#             */
/*   Updated: 2024/09/01 18:59:26 by amaassaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	ch_search;

	ch_search = (char)c;
	while (*s)
	{
		if (*s == ch_search)
			return ((char *)s);
		s++;
	}
	if (*s == ch_search)
		return ((char *)s);
	return (NULL);
}
