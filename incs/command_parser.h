/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 16:36:17 by zanikin           #+#    #+#             */
/*   Updated: 2024/05/08 19:06:31 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_PARSER_H
# define COMMAND_PARSER_H
# include <sys/types.h>
# include "libft.h"

# define MSE_OP_PAR 423
# define MSE_NO_OR_OP2 424
# define MSE_NO_OR_OP1 425
# define MSE_NO_AND_OP2 426
# define MSE_NO_AND_OP1 427
# define MSE_EM_PAR 428

# define MSPO_NONE 0
# define MSPO_OR 1
# define MSPO_AND 2

int	exec_cmd_str(const char *str, size_t len, int prev_op);
int	exec_cmd(const char *cmd, size_t len);

#endif
