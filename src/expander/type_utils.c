/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:10:18 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/08/22 09:11:34 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	word_all_single(unsigned int f)
{
	return ((f & QF_ALL_SQ));
}

bool	word_all_double(unsigned int f)
{
	return ((f & QF_ALL_DQ));
}

bool	word_has_quotes(unsigned int f)
{
	return ((f & (QF_HAS_SQ | QF_HAS_DQ)));
}

bool	word_mixed(unsigned int f)
{
	return ((f & QF_MIXED));
}

bool	word_hd_quote(unsigned int f)
{
	return ((f & QF_HD_QUOTED));
}
