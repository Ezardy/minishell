/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 19:58:01 by zanikin           #+#    #+#             */
/*   Updated: 2024/05/16 20:01:51 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_handler.h"
#include "libft.h"
#include <stdlib.h>

static int	fill_quotes(t_quotes *qt, const char *stmp, const char *dtmp);
static int	get_next_quotes(const char **pstr, t_qlist ***pl, const char *stmp,
				const char *dtmp);

t_quotes	*get_cor_quotes(const char *pc, t_qlist *qt)
{
	t_quotes	*cqt;

	cqt = NULL;
	if (qt && qt->content->q1 < pc)
	{
		while (qt && (qt->content->q2 <= pc))
			qt = qt->next;
		if (qt && qt->content->q1 < pc)
			cqt = qt->content;
	}
	return (cqt);
}

int	get_all_quotes(const char *str, t_qlist **l)
{
	int			error;
	const char	*stmp;
	const char	*dtmp;
	t_qlist		**root;

	error = 0;
	*l = NULL;
	root = l;
	while (!error && str)
	{
		stmp = ft_strchr(str, MST_DQ);
		dtmp = ft_strchr(str, MST_SQ);
		if (stmp || dtmp)
			error = get_next_quotes(&str, &l, stmp, dtmp);
		else
			str = NULL;
	}
	if (error)
		ft_lstclear((t_list **)root, free);
	return (error);
}

static int	get_next_quotes(const char **pstr, t_qlist ***pl, const char *stmp,
				const char *dtmp)
{
	t_quotes	*qt;
	int			error;
	t_qlist		**l;

	l = *pl;
	qt = (t_quotes *)malloc(sizeof(t_quotes));
	error = (qt == NULL) * MSE_ALLOC;
	if (!error)
	{
		*l = (t_qlist *)malloc(sizeof(t_qlist));
		(*l)->next = NULL;
		error = (*l == NULL) * MSE_ALLOC;
		if (!error)
		{
			(*l)->content = qt;
			error = fill_quotes(qt, stmp, dtmp);
			*pstr = qt->q2 + 1;
			l = &((*l)->next);
		}
	}
	*pl = l;
	return (error);
}

static int	fill_quotes(t_quotes *qt, const char *stmp, const char *dtmp)
{
	if (stmp && (dtmp == NULL || stmp < dtmp))
		qt->q1 = stmp;
	else
		qt->q1 = dtmp;
	qt->q2 = ft_strchr(qt->q1 + 1, qt->q1[0]);
	return ((qt->q2 == NULL) * (MSE_OP_DQ * (qt->q1[0] == MST_DQ)
			+ MSE_OP_SQ * (qt->q1[0] == MST_SQ)));
}
