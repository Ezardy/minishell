/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:55:29 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/10 21:06:40 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// prompt; history; run executables; redirections; ' and ";
// pipes; env vars; $? ; ctrl-C; ctrl-D; ctrl-\; built-ins

int			is_builtin(char *str);
int			is_cmd(char *str);
int			pipe_count(char *str);
static void	sigact_handler(int signum, siginfo_t *info, void *context);

static int	sa_code = 0;

int	main(int argc, char **argv, char **envp)
{
	char				*str;
	struct sigaction	sa;
	int					exit_status;


	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sigact_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	while (1)
	{
		str = readline("minishell$ ");
		if (ft_strlen(str) > 0)
			add_history(str);
		exit_status = pipex(str, envp);
		free(str);
	}
}

static void	sigact_handler(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	sa_code = signum;
}
