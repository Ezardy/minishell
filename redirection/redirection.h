/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 13:24:16 by zanikin           #+#    #+#             */
/*   Updated: 2024/07/02 21:33:38 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H
# include "export/t_export.h"
# include "t_fd.h"

char	**remove_redirections(const char *str, t_fd *fds, t_export **evl,
			int error);
int		count_heredoc(const char **pipes);
#endif
