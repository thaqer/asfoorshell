/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaassaf <amaassaf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 18:59:47 by amaassaf          #+#    #+#             */
/*   Updated: 2024/09/15 18:36:17 by amaassaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	ch_search;
	char	*ptr;

	ptr = NULL;
	ch_search = (char)c;
	while (*s)
	{
		if (*s == ch_search)
			ptr = (char *)s;
		s++;
	}
	if (ch_search == '\0')
		ptr = (char *)s;
	return (ptr);
}
