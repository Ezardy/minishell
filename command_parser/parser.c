/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 16:03:18 by zanikin           #+#    #+#             */
/*   Updated: 2024/05/09 18:03:09 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_parser.h"

static char	*ft_strrnchr(const char *str, char c, size_t len);
static int	exec_continuation_order(const char *str, size_t len,
				const char *lop, int is_or);
static int	exec_continuation(const char *str, size_t len, int prev_op);

int	exec_cmd_str(const char *str, size_t len, int prev_op)
{
	char	*par2;
	int		error;

	while (str[0] == ' ')
	{
		str++;
		len--;
	}
	if (str[0] == '(')
	{
		par2 = ft_strrnchr(str, ')', len);
		if (par2 && par2 - str > 1)
			error = exec_cmd_str(str + 1, par2 - str - 1, MSPO_NONE);
		else if (par2)
			error = MSE_EM_PAR;
		else
			error = MSE_OP_PAR;
	}
	else
		error = exec_continuation(str, len, prev_op);
	return (error);
}

static int	exec_continuation(const char *str, size_t len, int prev_op)
{
	char	*and;
	char	*or;
	int		error;

	and = ft_strnstr(str, "&&", len);
	or = ft_strnstr(str, "||", len);
	if (and && (!or || and < or))
		error = exec_continuation_order(str, len, and, 0);
	else if (or)
		error = exec_continuation_order(str, len, or, 1);
	else
	{
		if (str[0] == '\0')
		{
			if (prev_op == MSPO_OR)
				error = MSE_NO_OR_OP2;
			else
				error = MSE_NO_AND_OP2;
		}
		else
			error = exec_cmd(str, len);
	}
}

int	exec_cmd(const char *cmd, size_t len)
{

}

static int	exec_continuation_order(const char *str, size_t len,
				const char *lop, int is_or)
{
	int	error;
	int	tmp_error;

	error = (str == lop) * (is_or * MSE_NO_OR_OP1 + (!is_or) * MSE_NO_AND_OP1);
	if (!error)
	{
		tmp_error = exec_cmd(str, lop - str);
		if (!is_or && tmp_error)
			error = tmp_error;
		else
			error = exec_cmd_str(lop + 2, len - (lop - str) - 1,
					(is_or * MSPO_OR + (!is_or) * MSPO_AND));
	}
	return (error);
}

static char	*ft_strrnchr(const char *str, char c, size_t len)
{
	char	*r;

	r = NULL;
	if (len--)
	{
		while (len && str[len] != c)
			len--;
		if (str[len] == c)
			r = str + len;
	}
	return (r);
}
