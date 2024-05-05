/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 16:36:17 by zanikin           #+#    #+#             */
/*   Updated: 2024/05/05 17:32:13 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_PARSER_H
# define COMMAND_PARSER_H
# include <sys/types.h>
# include "libft.h"

# define MS_STR 0
# define MS_AND 1
# define MS_OR 2
# define MS_PIPE 3
# define MS_REDINP 4
# define MS_REDOUT 5

typedef struct s_token
{
	char	*t;
	size_t	l;
}	t_token;

typedef struct s_btree
{
	struct s_btree	*l;
	struct s_btree	*r;
	int				op;
	t_token			*t;
}	t_btree;

t_btree	*btree_new(void *content);
void	btreenode_add(t_btree *node, void *left, void *right);
#endif
