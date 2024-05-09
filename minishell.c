/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:55:29 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/09 15:13:35 by zanikin          ###   ########.fr       */
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
		if (args.p_count != 0)
		{
			fd = (int *) malloc(sizeof(int) * (p_count * 2));
			j = 0;
			i = 0;
			while (i < p_count)
			{
				pipe(fd + i * 2);
				i++;
			}
			pipex(args, fd);
			close_all(fd, args.p_count);
			leave_children();
			free(fd);
		}
		free_arr(words1);
		free(str);
	}
}

static void	sigact_handler(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	sa_code = signum;
}

int	is_builtin(char *str)
{
	int	i;
	int	ans;

	i = 0;
	ans = 0;
	char	*buf[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
	while (buf[i] != NULL)
	{
		if (ft_strncmp(str, buf[i], ft_strlen(buf[i])) == 0)
			ans = 1;
		i++;
	}
	return (ans);
}

int	is_cmd(char *str)
{
	int	i;
	int	ans;

	i = 0;
	ans = 0;
	if (access(str, F_OK | R_OK) == 0 || is_builtin(str) == 1)
		ans = 1;
	return (ans);
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
