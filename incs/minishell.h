/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazari <mamazari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 18:44:22 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/13 14:32:18 by mamazari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include "readline/history.h"
# include "readline/readline.h"
# include "libft.h"
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <fcntl.h>
# include <limits.h>
# include <errno.h>
# include <string.h>
# include "builtins.h"

char	**my_split(char *str, char *charset);
char	*search_path(char *cmd, char **path);
char	*get_str(char **strs2, char *cmd);
void	free_arr(char **av);
void	leave_children(void);
void	close_all(int fd[], int argc);

typedef struct s_args
{
	char	**argv;
	char	**envp;
	int		p_count;
	int		exit_code;
}		t_args;

int		pipex(t_args args, int fd[]);
int		do_execve_first(t_args args, int fd[]);
void	do_execve_fd(t_args args, int fd[], int *i, int *j);
int		do_execve_last(t_args args, int fd[], int *i);

#endif
