/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:21:23 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/09 17:57:53 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	pipe_count(char *str);

int	pipex(t_pipex args, int fd[])
{
	int	i;
	int	j;
	int	p;
	int	status;
	int	exit_status;

	if (args.p_count == 0)
		exit_status = do_execve_first(args, fd);
	else
		do_execve_first(args, fd);
	j = 1;
	i = 0;
	while (j < args.p_count)
	{
		p = fork();
		if (p == 0)
			do_execve_fd(args, fd, &i, &j);
		j++;
		i += 2;
	}
	if (args.p_count > 0)
		exit_status = do_execve_last(args, fd, &i);
	return (exit_status);
}

static int	pipe_count(char *str)
{
	int	i;
	int	c;

	i = 0;
	c = 0;
	while (str[i])
	{
		if (str[i] == '|')
			c++;
		i++;
	}
	return (c);
}
