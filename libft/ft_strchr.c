/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 17:10:16 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/27 12:07:45 by dqaddomi         ###   ########.fr       */
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
