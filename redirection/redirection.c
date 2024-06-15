/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 13:25:26 by zanikin           #+#    #+#             */
/*   Updated: 2024/06/15 13:24:22 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "t_fd.h"
#include "t_env_exp.h"
#include "quotes/quotes.h"
#include "t_hdlst.h"
#include "common/common.h"

char		*get_redir_arg(const char **str, t_export **evl, int error);
int			count_cmd_str(const char *str, size_t *size);

static char	*redirect(const char *str, t_fd *fds, t_hdlst **dels,
				t_env_exp *env_exp);
static char	*heredoc(const char *str, t_fd *fds, t_hdlst **dels,
				t_env_exp *env_exp);
static char	*read_from_file(const char *str, t_fd *fds, t_env_exp *env_exp);
static char	*write_to_file(const char *str, t_fd *fds, int oflags,
				t_env_exp *env_exp);

char	**remove_redirections(const char *str, t_fd *fds, t_hdlst **dels,
			t_env_exp	*env_exp)
{
	char	*cmd;
	char	**splitted_cmd;
	size_t	cmd_size;
	size_t	i;

	*dels = NULL;
	cmd = NULL;
	i = 0;
	if (count_cmd_str(str, &cmd_size))
		cmd = (char *)malloc(sizeof(char) * (cmd_size + 1));
	if (cmd)
	{
		track_quote(str, '\0', 0);
		while (i < cmd_size)
		{
			if (!track_quote(NULL, '\0', 0) && (*str == '<' || *str == '>'))
				str = redirect(str, fds, dels, env_exp);
			else
				cmd[i++] = *str++;
		}
		cmd[i] = '\0';
		splitted_cmd = quoted_split(cmd, "\t\n\v\f\r ");
		free(cmd);
	}
	return (splitted_cmd);
}

static char	*redirect(const char *str, t_fd *fds, t_hdlst **dels,
				t_env_exp *env_exp)
{
	if (*str == '<' && str[1] == '<')
		str = heredoc(str + 2, fds, dels, env_exp);
	else if (*str == '<')
		str = read_from_file(str + 1, fds, env_exp);
	else if (str[1] == '>')
		str = write_to_file(str + 2, fds, O_APPEND, env_exp);
	else
		str = write_to_file(str + 1, fds, 0, env_exp);
	return (str);
}

static char	*read_from_file(const char *str, t_fd *fds, t_env_exp *env_exp)
{
	char	*arg;
	int		fd;

	arg = get_redir_arg(&str, env_exp->evl, env_exp->error);
	fd = open(arg, O_RDONLY);
	if (fd < 0)
		print_error_msg(strerror(errno), arg);
	else
	{

	}
	return (str);
}

static char	*write_to_file(const char *str, t_fd *fds, int oflags,
				t_env_exp *env_exp)
{
	char	*arg;
	int		fd;

	arg = get_redir_arg(&str, env_exp->evl, env_exp->error);
	fd = open(arg, oflags | O_WRONLY | O_CREAT);
	if (fd < 0)
		print_error_msg(strerror(errno), arg);
	else
	{

	}
	return (str);
}

static char	*heredoc(const char *str, t_fd *fds, t_hdlst **dels,
				t_env_exp *env_exp)
{

}
