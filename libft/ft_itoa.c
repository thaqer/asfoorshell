/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaassaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:00:35 by amaassaf          #+#    #+#             */
/*   Updated: 2024/09/09 13:01:55 by amaassaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	t_dig(long n)
{
	size_t	i;

	i = 0;
	if (n <= 0)
	{
		n = -n;
		i++;
	}
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	size_t	size;
	long	num;
	char	*s;

	num = n;
	size = t_dig(num);
	s = malloc(sizeof(char) * (size + 1));
	if (!s)
		return (NULL);
	s[size] = '\0';
	if (num < 0)
	{
		s[0] = '-';
		num = -num;
	}
	if (num == 0)
		s[0] = '0';
	while (num > 0)
	{
		s[size - 1] = num % 10 + 48;
		num = num / 10;
		size--;
	}
	return (s);
}
/*int	main(void)
{
	printf("%s", ft_itoa(12345));
}*/
