/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:33:03 by zanikin           #+#    #+#             */
/*   Updated: 2024/05/17 15:14:13 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_handler.h"
#include "libft.h"

static const char	*strrn2chr(const char *str, size_t len)

int	exec_cmd_str_1(const char *str, size_t len, t_qlist *qt)
{
	int			error;
	const char	*op_par;
	const char	*and;
	const char	*or;

	error = 0;
	while (str[len - 1] == ' ')
		len--;
	if (str[len - 1] == ')')
	{
		op_par = ft_strnstr(str, "(", len - 1);
		error = (op_par == NULL) * MSE_OP_PAR;
		if (!error)
			error = exec_cmd_str_1(op_par + 1, len - (op_par - str) - 2, qt);
	}
	if (error < MSE_OP_PAR || error > MSE_NO_CTRL)
	{
		and = ft_str
	}
}
