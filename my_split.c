/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 13:17:38 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/16 19:47:29 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "command_handler.h"

static int	is_separator(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i] != '\0')
	{
		if (c == charset[i])
		{
			return (1);
		}
		i++;
	}
	return (0);
}

static int	word_count(const char *str, char *charset, size_t size, t_qlist *qt)
{
	int	i;
	int	word;
	int	count;

	i = 0;
	word = 0;
	count = 0;
	while (str[i] && size--)
	{
		if (!(!get_cor_quotes(str + i, qt) && is_separator(str[i], charset))
			&& word == 0)
		{
			count++;
			word = 1;
		}
		else if (word)
			word = 0;
		i++;
	}
	return (count);
}

static int	len(const char *str, char *charset)
{
	int	i;

	i = 0;
	while (str[i] && !is_separator(str[i], charset))
		i++;
	return (i);
}

static char	*ft_strdup(const char *dest, int len)
{
	int		i;
	char	*str;

	i = 0;
	str = (char *) malloc(sizeof(1) * (len + 1));
	while (i < len)
	{
		str[i] = dest[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	**my_split(const char *str, char *charset, size_t size, t_qlist *qt)
{
	int		i;
	int		j;
	int		word;
	char	**ans;

	i = 0;
	j = 0;
	word = 0;
	ans = malloc(sizeof(char *) * (word_count(str, charset, size, qt) + 1));
	while (str[i] && size--)
	{
		if (!(!get_cor_quotes(str + i, qt) && is_separator(str[i], charset))
			&& word == 0)
		{
			word = 1;
			ans[j] = ft_strdup(&str[i], len(&str[i], charset));
			j++;
		}
		else if (word)
			word = 0;
		i++;
	}
	ans[j] = NULL;
	return (ans);
}
