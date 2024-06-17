/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:03:46 by zanikin           #+#    #+#             */
/*   Updated: 2024/06/17 10:43:23 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>

#include "t_env_exp.h"
#include "libft/libft.h"
#include "t_hdlst.h"
#include "common/common.h"
#include "t_fd.h"

static const char	*rw(const char *str, int *fd, t_env_exp *env_exp,
						int flags);

const char	*read_from_file(const char *str, t_fd *fds, t_env_exp *env_exp)
{
	return (rw(str, &fds->rfd, env_exp, O_RDONLY));
}

const char	*write_to_file(const char *str, t_fd *fds, int oflags,
				t_env_exp *env_exp)
{
	return (rw(str, &fds->wfd, env_exp, oflags | O_WRONLY | O_CREAT));
}

const char	*heredoc(const char *str, t_fd *fds, t_hdlst **dels,
				t_env_exp *env_exp)
{
	t_list	*new;
	char	*arg;

	arg = get_redir_arg(&str, env_exp->evl, env_exp->error);
	if (arg)
	{
		new = ft_lstnew(arg);
		if (new)
			ft_lstadd_back((t_list **)dels, new);
		else
		{
			free(arg);
			str = NULL;
		}
	}
	else
		str = NULL;
	return (str);
}

static const char	*rw(const char *str, int *fd, t_env_exp *env_exp, int flags)
{
	char	*arg;
	int		nfd;

	arg = get_redir_arg(&str, env_exp->evl, env_exp->error);
	if (arg)
	{
		nfd = open(arg, flags);
		if (nfd < 0)
			print_error_msg(strerror(errno), arg);
		else
		{
			if (*fd != -1)
				close(*fd);
			*fd = nfd;
		}
		free(arg);
	}
	else
		str = NULL;
	return (str);
}