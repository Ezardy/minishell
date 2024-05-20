/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 16:03:18 by zanikin           #+#    #+#             */
/*   Updated: 2024/05/20 00:11:25 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_handler.h"
#include "libft.h"

static int	is_outside(const char *str, const char const *pc);

int	exec_cmd_str(const char *str, size_t len, int prev_op, t_qlist *qt)
{

}

static int	is_outside(const char *str, const char const *pc)
{
	long		pars;

	pars = 0;
	while (*str != pc)
	{
		if (*str == '(')
			pars++;
		else if (*str == ')')
			pars--;
		str++;
	}
	return (pars == 0);
}
