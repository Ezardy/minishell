/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:02:53 by zanikin           #+#    #+#             */
/*   Updated: 2024/06/03 19:07:26 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "libft/libft.h"

static void	set_flag(char **strs, int *i, int *flag);
static int	split_len(char **split);

int	echo(char **strs)
{
	int		flag;
	int		i;

	flag = 0;
	i = 1;
	if (split_len(strs) > 1)
	{
		set_flag(strs, &i, &flag);
		while (strs[i] != NULL)
		{
			printf("%s", strs[i]);
			if (strs[i + 1] != NULL)
				printf(" ");
			i++;
		}
	}
	if (flag == 0)
		printf("\n");
	return (0);
}

static int	split_len(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
		i++;
	return (i);
}

static void	set_flag(char **strs, int *i, int *flag)
{
	char	*trimmed;

	while (strs[*i] && strs[*i][0] == '-')
	{
		trimmed = ft_strtrim(strs[*i], "n");
		if (!(ft_strlen(trimmed) == 1 && ft_strncmp(trimmed, "-", 1) == 0))
		{
			free(trimmed);
			break ;
		}
		else
		{
			*flag = 1;
			free(trimmed);
		}
		(*i)++;
	}
}