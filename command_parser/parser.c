/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 16:03:18 by zanikin           #+#    #+#             */
/*   Updated: 2024/05/05 16:54:51 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_parser.h"

t_list	*tokenize(char *str)
{
	t_list	*tl;
	t_token	*tmp;

	tl = NULL;
	tmp = (t_token *)malloc(sizeof(t_token));
	if (tmp)
		tl = ft_lstnew(tmp);
	if (tl)
	{
		while (*str)
		{

		}
	}
	return (tl);
}
