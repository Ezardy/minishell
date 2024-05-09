/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:55:29 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/09 17:21:24 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// prompt; history; run executables; redirections; ' and ";
// pipes; env vars; $? ; ctrl-C; ctrl-D; ctrl-\; built-ins

int			is_builtin(char *str);
int			is_cmd(char *str);
void		do_execve_red(char *strs, char **envp, int file);
int			create_fd(char *name);
int			pipe_count(char *str);
static void	sigact_handler(int signum, siginfo_t *info, void *context);

static int	sa_code = 0;

int	main(int argc, char **argv, char **envp)
{
	char				*str;
	char				**words1;
	char				*command;
	char				**splitted;
	int					j;
	int					i;
	int					p_count;
	t_args				args;
	int					*fd;
	struct sigaction	sa;
	int					exit_status;

	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sigact_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	args.envp = envp;
	while (1)
	{
		str = readline("minishell$ ");
		if (ft_strlen(str) > 0)
			add_history(str);
		j = 0;
		words1 = my_split(str, "|");
		args.argv = words1;
		p_count = pipe_count(str);
		args.p_count = p_count;
		fd = (int *) malloc(sizeof(int) * (p_count * 2));
		j = 0;
		i = 0;
		while (i < p_count)
		{
			pipe(fd + i * 2);
			i++;
		}
		exit_status = pipex(args, fd);
		close_all(fd, args.p_count);
		leave_children();
		free(fd);
		free_arr(args.argv);
		free(str);
		printf("exit status: %d\n", exit_status);
	}
}

static void	sigact_handler(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	sa_code = signum;
}

void	do_execve(t_args arg)
{
	char	**av;
	char	*command;

	if (fork() == 0)
	{
		av = ft_split(arg.argv[0], ' ');
		if (av[0][0] == '/' || (av[0][0] == '.' && av[0][1] == '/'))
			command = av[0];
		else
			command = search_path(av[0], arg.envp);
		if (execve(command, av, arg.envp) == -1)
		{
			perror(command);
			exit(1);
		}
	}
	wait(NULL);
}

void	do_execve_red(char *strs, char **envp, int file)
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

int	create_fd(char *name)
{
	int	fd;

	fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

int	pipe_count(char *str)
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
