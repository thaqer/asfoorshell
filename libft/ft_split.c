/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:21:49 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/27 12:07:45 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_wordcount(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

static void	ft_freeslices(char **slices)
{
	size_t	i;

	i = 0;
	while (slices[i] != NULL)
		free(slices[i++]);
	free(slices);
}

int	ft_fill_slices(char **slices, char const *s, char c)
{
	size_t	i;
	size_t	x;

	x = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		i = 0;
		while (s[i] && s[i] != c)
			i++;
		if (i > 0)
		{
			slices[x] = ft_substr(s, 0, i);
			if (!slices[x++])
				return (0);
		}
		s += i;
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**slices;
	size_t	wordnum;

	wordnum = ft_wordcount(s, c);
	slices = ft_calloc(wordnum + 1, sizeof(char *));
	if (!slices)
		return (NULL);
	if (!ft_fill_slices(slices, s, c))
	{
		ft_freeslices(slices);
		return (NULL);
	}
	return (slices);
}

// int	main(void)
//{
//
//	char	s1[] = "   Hi               Hello    Ahlein  Marhaba   ";
// 	char	**slices;
//	int		i;
//
//	slices = ft_split(s1, ' ');
// 	i = 0;
// 	while (slices[i] != NULL)
// 	{
// 		printf("%s\n", slices[i]);
// 		i++;
// 	}
//	ft_freeslices(slices);
//}
/*
void	ft_strncpy(char *dest, const char *src, size_t n)
{
	while (n-- && (*dest++ = *src++))
		;
	*dest = '\0';
}

size_t	count_words(char *str)
{
	int	wc;

	wc = 0;
	while (*str)
	{
		while (*str == ' ' || *str == '\t' || *str == '\n')
			str++;
		wc++;
		while (*str && *str != ' ' && *str != '\t' && *str != '\n')
			str++;
		if (*str)
			str++;
	}
	return(wc);
}

void	go_free(char **result)
{
	size_t	i;

	i = 0;
	while (result[i] != NULL)
		free(result[i++]);
	free(result);
}

char	**ft_split(char *str)
{
	int		nb_words;
	char	**result;
	int		i;
		char *start;
		char *end;
	size_t	size;

	nb_words = 0;
	nb_words = count_words(str);
	result = malloc(sizeof(char *) * (nb_words + 1));
	if (!result)
		return (NULL);
	result[nb_words] = NULL;
	i = 0;
	while (*str)
	{
		while (*str == ' ' || *str == '\t' || *str == '\n')
			str++;
		start = str;
		while (*str && *str != ' ' && *str != '\t' && *str != '\n')
			str++;
		end = str;
		if (start == end)
			break ;
		size = end - start + 1;
		result[i] = malloc(sizeof(char) * size);
		if (!result[i])
				go_free(result);
		ft_strncpy(result[i], start, size);
		if (*str)
			str++;
		i++;
	}
	return (result);
}
*/
