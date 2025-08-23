/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaassaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 16:02:59 by amaassaf          #+#    #+#             */
/*   Updated: 2024/08/27 16:52:00 by amaassaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
	{
		return (1);
	}
	return (0);
}
/*
int	main(void)
{
	int	c = 129;
	if (ft_isascii(c))
	{
		printf("character %c is an ASCII character", c);
	}
	printf("%d", ft_isascii(c));
	return (0);
}*/
