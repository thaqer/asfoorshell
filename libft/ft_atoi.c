/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dqaddomi <dqaddomi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:40:04 by dqaddomi          #+#    #+#             */
/*   Updated: 2025/08/27 12:07:45 by dqaddomi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_atoi(const char *str)
{
	int	num;
	int	sign;

	sign = 1;
	num = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
	{
		str++;
	}
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= 48 && *str <= 57)
	{
		num = num * 10 + (*str - 48);
		str++;
	}
	return (num * sign);
}
/*
int	main(void)
{
	// Test cases
	const char *test_cases[] = {
		"123",                // Basic positive number
		"-123",               // Basic negative number
		"+123",               // Positive number with explicit plus sign
		"   456",             // Number with leading spaces
		"-   789",
			// Negative number with spaces in between (should stop at space)
		"42 is the answer",   // Number followed by text
		"   +42 with text",   // Number with leading spaces and text after
		"text 123",           // Text before number
		"0",                  // Zero
		"-0",                 // Negative zero
		"+0",                 // Positive zero
		"2147483647",
			// Large positive number (max value for 32-bit int)
		"--+2147483648",
			// Large negative number (min value for 32-bit int)
		"++2147483648",         // Number greater than 32-bit int max value
		"--2147483649",        // Number less than 32-bit int min value
		NULL                  // End of test cases
	};

	// Loop through test cases and print results
	for (int i = 0; test_cases[i] != NULL; i++) {
		printf("Input: \"%s\" => Output: %d\n", test_cases[i],
			ft_atoi(test_cases[i]));
	}

	return (0);
}*/
