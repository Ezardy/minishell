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

static const char	*ft_strrnchr(const char *str, char c, size_t len);
static int			exec_continuation_order(t_string *str, const char *lop,
						int prev_op, t_qlist *qt);
static int			exec_continuation(t_string *str, int prev_op,
						t_qlist *qt);
static size_t		skip_spaces(const char **pstr);

int	exec_cmd_str(const char *str, size_t len, int prev_op, t_qlist *qt)
{
	const char	*par2;
	int			error;
	t_string	substr;

	error = 0;
	len -= skip_spaces(&str);
	if (str[0] == '(')
	{
		par2 = ft_strrnchr(str, ')', len);
		error = MSE_OP_PAR;
		if (par2 && par2 - str > 1)
		{
			error = exec_cmd_str(str + 1, (par2 - str) - 1, MSPO_NONE, qt);
			str = par2 + 1;
			len -= par2 - str + 1;
		}
		else if (par2)
			error = MSE_EM_PAR;
		prev_op = MSPO_PAR | error;
	}
	substr.l = len;
	substr.s = str;
	if (str[0] && (error < MSE_OP_PAR || error > MSE_NO_CTRL))
		error = exec_continuation(&substr, prev_op, qt);
	return (error);
}

static size_t	skip_spaces(const char **pstr)
{
	size_t	skiped;

	skiped = 0;
	while (pstr[0][0] == ' ')
	{
		skiped++;
		pstr[0]++;
	}
	return (skiped);
}

static int	exec_continuation(t_string *str, int prev_op, t_qlist *qt)
{
	char	*and;
	char	*or;
	int		error;

	and = ft_strnstr(str->s, "&&", str->l);
	or = ft_strnstr(str->s, "||", str->l);
	if (and && !get_cor_quotes(and, qt) && (!or || get_cor_quotes(or, qt)
			|| and < or))
		error = exec_continuation_order(str, and, prev_op, qt);
	else if (or && !get_cor_quotes(or, qt))
		error = exec_continuation_order(str, or, prev_op, qt);
	else
	{
		if (str->s[0] == '\0')
		{
			if (prev_op == MSPO_OR)
				error = MSE_NO_OR_OP2;
			else
				error = MSE_NO_AND_OP2;
		}
		else
			error = exec_cmd(str->s, str->l, qt);
	}
	return (error);
}

static int	exec_continuation_order(t_string *str, const char *lop, int prev_op,
				t_qlist *qt)
{
	int			error;
	int			tmp_error;

	error = (str->s == lop) * ((*lop == '|') * MSE_NO_OR_OP1
			+ (*lop == '&') * MSE_NO_AND_OP1);
	if (!error || prev_op & MSPO_PAR)
	{
		if (prev_op & MSPO_PAR)
			tmp_error = prev_op ^ MSPO_PAR;
		else
			tmp_error = exec_cmd(str->s, lop - str->s, qt);
		if (*lop == '&' && tmp_error || *lop == '|' && !tmp_error)
			error = tmp_error;
		else
			error = exec_cmd_str(lop + 2, str->l - (lop - str->s) - 1,
					((*lop == '|') * MSPO_OR + (*lop == '&') * MSPO_AND), qt);
	}
	return (error);
}

static const char	*ft_strrnchr(const char *str, char c, size_t len)
{
	const char	*r;

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
