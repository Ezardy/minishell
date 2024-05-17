/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 10:55:29 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/17 17:12:32 by zanikin          ###   ########.fr       */
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
	t_qlist				*qt;

	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sigact_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	while (1)
	{
		str = readline("minishell$ ");
		if (*str)
		{
			add_history(str);
			exit_status = get_all_quotes(str, &qt);
			if (!exit_status)
				exit_status = exec_cmd_str(str, ft_strlen(str), MSPO_NONE, qt);
		}
		else
			exit_status = 0;
		free(str);
	}
}

int	exec_cmd(const char *cmd, size_t len, t_qlist *qt)
{
	(void)qt;
	printf("exec: %.*s\n", (int)len, cmd);
	return (0);
}

static void	sigact_handler(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	sa_code = signum;
}
