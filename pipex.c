/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:21:23 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/15 19:02:14 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	pipe_count(const char *str);
static int	pipex_exec(t_pipex args, int fd[]);
static void	do_execve_red(char *strs, char **envp, int file);

int	pipex(const char *str, size_t size, char **envp, t_qlist *qt)
{
	t_pipex	args;
	int		p_count;
	int		*fd;
	int		i;
	int		exit_status;

	args.envp = envp;
	args.argv = my_split(str, "|", size, qt);
	p_count = pipe_count(str);
	args.p_count = p_count;
	fd = (int *) malloc(sizeof(int) * (p_count * 2));
	i = 0;
	while (i < p_count)
	{
		pipe(fd + i * 2);
		i++;
	}
	exit_status = pipex_exec(args, fd);
	close_all(fd, args.p_count);
	leave_children();
	free(fd);
	free_arr(args.argv);
	return (exit_status);
}

static int	pipex_exec(t_pipex args, int fd[])
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

static int	pipe_count(const char *str)
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

static void	do_execve_red(char *strs, char **envp, int file)
{
	char	**av;
	char	*command;

	if (fork() == 0)
	{
		av = ft_split(strs, ' ');
		command = search_path(av[0], envp);
		dup2(file, 1);
		if (execve(command, av, envp) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
}
