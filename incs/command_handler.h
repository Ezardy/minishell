/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 16:36:17 by zanikin           #+#    #+#             */
/*   Updated: 2024/05/17 18:02:43 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HANDLER_H
# define COMMAND_HANDLER_H
# include <sys/types.h>

# define MSE_OP_PAR 423
# define MSE_NO_OR_OP2 424
# define MSE_NO_OR_OP1 425
# define MSE_NO_AND_OP2 426
# define MSE_NO_AND_OP1 427
# define MSE_EM_PAR 428
# define MSE_OP_DQ 429
# define MSE_OP_SQ 430
# define MSE_ALLOC 431
# define MSE_NO_CTRL 500

# define MSPO_NONE 0
# define MSPO_OR 0b00100000000000000000000000000000
# define MSPO_AND 0b01000000000000000000000000000000
# define MSPO_PAR 0b10000000000000000000000000000000

# define MST_SQ '\''
# define MST_DQ '"'

typedef struct s_string
{
	const char	*s;
	size_t		l;
}	t_string;

typedef struct s_quotes
{
	const char	*q1;
	const char	*q2;
}	t_quotes;

typedef struct s_qlist
{
	t_quotes		*content;
	struct s_qlist	*next;
}	t_qlist;

int			exec_cmd_str(const char *str, size_t len, int prev_op, t_qlist *qt);
int			exec_cmd(const char *cmd, size_t len, t_qlist *qt);
int			get_all_quotes(const char *str, t_qlist **l);
t_quotes	*get_cor_quotes(const char *pc, t_qlist *qt);

#endif
