/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:21:23 by mamazari          #+#    #+#             */
/*   Updated: 2024/06/04 19:07:49 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "pwd/pwd.h"
#include "echo/echo.h"
#include "exit/exit.h"
#include "expansion/expansion.h"
#include "common/common.h"
#include "export/export.h"
#include "t_args.h"
#include "t_fd.h"

char		*search_path(char *cmd, t_export **env);
int			handle_unset(t_args *args, char **av);
int			handle_export(t_args *args, char **av);
int			handle_cd(t_args *args, char **av);
void		append_pid(int p, t_args *args);

static void	run_external_command(t_args *args, t_fd *p, char **av);
static void	handle_pipe(int j, t_args *args, t_fd *p, char **av);
static void	execute_command(char *first, char **av, char **envp, t_args *args);
static int	run_command_if_builtin(char **av, t_args *args, int *code);

void	pipex(t_args *args)
{
	t_fd	p;
	int		j;
	char	**av;

	p.tempin = dup(0);
	p.tempout = dup(1);
	p.fdin = dup(p.tempin);
	j = 0;
	while (j < args->p_count + 1)
	{
		dup2(p.fdin, 0);
		close(p.fdin);
		av = quoted_split(args->argv[j], ' ');
		if (expand_list(av, &args->env_list, args->exit_code))
			handle_pipe(j, args, &p, av);
		free_arr(av);
	}
	dup2(p.tempin, 0);
	dup2(p.tempout, 1);
	close(p.tempin);
	close(p.tempout);
}

static void	handle_pipe(int j, t_args *args, t_fd *p, char **av)
{
	pid_t	pid;
	int		error;

	if (j == args->p_count)
		p->fdout = dup(p->tempout);
	else
	{
		pipe(p->fd);
		p->fdout = p->fd[1];
		p->fdin = p->fd[0];
	}
	dup2(p->fdout, 1);
	close(p->fdout);
	if (run_command_if_builtin(av, args, &error))
	{
		pid = fork();
		if (pid == 0)
			exit(error);
		else
			append_pid(pid, args);
	}
	else
		run_external_command(args, p, av);
}

static int	run_command_if_builtin(char **av, t_args *args, int *code)
{
	int		ans;

	*code = 0;
	ans = 1;
	if (ft_strlen(av[0]) == 6 && ft_strncmp("export", av[0], 6) == 0)
		*code = handle_export(args, av);
	else if (ft_strlen(av[0]) == 3 && ft_strncmp("env", av[0], 3) == 0)
		print_list(&args->env_list, 0);
	else if (ft_strlen(av[0]) == 5 && ft_strncmp("unset", av[0], 5) == 0)
		*code = handle_unset(args, av);
	else if (ft_strlen(av[0]) == 4 && ft_strncmp("echo", av[0], 4) == 0)
		*code = echo(&av[0]);
	else if (ft_strlen(av[0]) == 3 && ft_strncmp("pwd", av[0], 3) == 0)
		free(pwd(1));
	else if (ft_strlen(av[0]) == 2 && ft_strncmp("cd", av[0], 2) == 0)
		*code = handle_cd(args, av);
	else if (ft_strlen(av[0]) == 4 && ft_strncmp("exit", av[0], 4) == 0)
		shell_exit(av[1]);
	else
		ans = 0;
	return (ans);
}

static void	execute_command(char *first, char **av, char **envp, t_args *args)
{
	char		*command;
	struct stat	file_info;

	if (stat(first, &file_info) != -1 && file_info.st_mode & S_IFMT == S_IFDIR)
	{
		print_error_msg("is a directory\n", first);
		exit(126);
	}
	command = search_path(first, &args->export_list);
	if (command == NULL || access(command, F_OK) != 0)
	{
		print_error_msg("command not found\n", first);
		exit(127);
	}
	if (access(command, X_OK) != 0)
	{
		print_error_msg("Permission denied\n", first);
		exit(126);
	}
	if (execve(command, av, envp) == -1)
		print_error_msg("error", first);
	exit(1);
}

static void	run_external_command(t_args *args, t_fd *p, char **av)
{
	int		pid;

	pid = fork();
	if (pid == 0)
	{
		close(p->fdin);
		close(p->fdout);
		if (av[0][0] == '/' && access(av[0], F_OK) != 0)
		{
			print_error_msg("No such file or directory\n", av[0]);
			exit(127);
		}
		execute_command(av[0], av, args->envp, args);
	}
	else if (pid == -1)
		print_error_msg("failed\n", "fork");
	else if (pid > 0)
		append_pid(pid, args);
}