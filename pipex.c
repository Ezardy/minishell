/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazari <mamazari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:21:23 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/20 18:16:06 by mamazari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/minishell.h"

void	setup_fds(t_temp *p)
{
	p->tempin = dup(0);
	p->tempout = dup(1);
	p->fdin = dup(p->tempin);
}

void	restore_fds(t_temp *p)
{
	dup2(p->tempin, 0);
	dup2(p->tempout, 1);
	close(p->tempin);
	close(p->tempout);
}

void	handle_builtin(char **av, t_args *args)
{
	int	i;
	int	p;
	int	ans;

	if (ft_strlen(av[0]) == 6 && ft_strncmp("export", av[0], 6) == 0)
	{
		i = 1;
		while (av[i])
			ans = my_export(args, av[i++]);
		if (i == 1)
			print_list(&args->export_list, 1);
	}
	else if (ft_strlen(av[0]) == 3 && ft_strncmp("env", av[0], 3) == 0)
		print_list(&args->env_list, 0);
	else if (ft_strlen(av[0]) == 5 && ft_strncmp("unset", av[0], 5) == 0)
	{
		i = 1;
		while (av[i])
		{
			my_unset(&args->export_list, av[i]);
			my_unset(&args->env_list, av[i++]);
		}
	}
	else if (ft_strlen(av[0]) == 4 && ft_strncmp("echo", av[0], 4) == 0)
		my_echo(&av[0]);
	else if (ft_strlen(av[0]) == 3 && ft_strncmp("pwd", av[0], 3) == 0)
		my_pwd();
	else if (ft_strlen(av[0]) == 2 && ft_strncmp("cd", av[0], 2) == 0)
	{
		if (!av[1])
			ans = my_cd("HOME", &args->export_list);
		else
			ans = my_cd(av[1], &args->export_list);
	}
	// p = fork();
	// if (p == 0)
	// {
	// 	if (ans != 0)
	// 		exit(1);
	// 	exit(0);
	// }
	// else
	// 	ft_lstadd_back(&args->pids, ft_lstnew(p));
}

char	*get_value_from_key(t_export **list, char *key)
{
	t_export	*temp;
	char		*ans;

	temp = *list;
	ans = NULL;
	while (temp)
	{
		if (ft_strncmp(key, temp->pair->key, \
			ft_strlen(temp->pair->key)) == 0)
		{
			ans = temp->pair->val;
			break ;
		}
		temp = temp->next;
	}
	return (ans);
}

int	my_strncpy(char *dest, char *src, int size)
{
	int	i;

	i = 0;
	while (src[i] && i < size)
	{
		dest[i] = src[i];
		i++;
	}
	return (i);
}

char	*get_ans(char *str, char *ans, char *home_dir)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = ft_strlen(home_dir);
	while (str[i])
	{
		if (str[i] == '~')
			j += my_strncpy(&ans[j], home_dir, len);
		else
			ans[j++] = str[i];
		i++;
	}
	ans[j] = '\0';
	return (ans);
}

char	*tilde_exp(char *str, t_export **list)
{
	int		i;
	int		j;
	int		count;
	char	*ans;
	char	*home_dir;

	i = 0;
	count = 0;
	home_dir = get_value_from_key(list, "HOME");
	if (!home_dir)
		return (NULL);
	while (str[i])
	{
		if (str[i++] == '~')
			count++;
	}
	i = 0;
	count = ft_strlen(home_dir) * count;
	while (str[i])
	{
		if (str[i++] != '~')
			count++;
	}
	ans = (char *) malloc(sizeof(char) * (count + 1));
	ans = get_ans(str, ans, home_dir);
	return (ans);
}

int	is_dir(char* path)
{
	struct stat	fileInfo;

	if (stat(path, &fileInfo) == -1)
		return (0);
	return (((fileInfo.st_mode) & S_IFMT) == S_IFDIR);
}

int	ft_str_is_numeric(char *str)
{
	int	i;
	int	ans;

	i = 0;
	ans = 1;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			ans = 0;
			break ;
		}
		i++;
	}
	return (ans);
}

void	handle_command(char **av, t_args *args)
{
	int		p;
	char	*command;
	char	*first;
	char	*err;
	char	*val_str;
	int		val_int;
	int		exit_code;
	
	if (ft_strchr(av[0], '~') != NULL)
	{
		first = tilde_exp(av[0], &args->export_list);
		if (!first)
			first = av[0];
	}
	else
		first = av[0];
	if (is_dir(first) != 0)
	{
		printf("minishell: %s: is a directory\n", first);
		exit(126);
	}
	command = search_path(first, &args->export_list);
	// if (ft_strlen(command) == 11 && ft_strncmp(command, "./minishell", 11) == 0)
	// {
	// 	val_str = get_value_from_key(&args->export_list, "SHLVL");
	// 	if (val_str && ft_str_is_numeric(val_str) == 1)
	// 	{
	// 		val_int = ft_atoi(val_str);
	// 	}
	// 	else
	// 		my_export(args, "SHLVL=1");
	// }
	p = fork();
	if (p == 0)
		execve(command, av, args->envp);
	// else		
	// err = strerror(errno);
	// if (ft_strncmp(err, "No such file or directory", 25) == 0 || \
	// 	ft_strncmp(err, "command not found", 17) == 0)
	// 	exit_code = 127;
	// else if (ft_strncmp(err, "Permission denied", 17) == 0)
	// 	exit_code = 126;
	// else	
	// 	exit_code = 1;
	// perror(command);
	// exit(exit_code);
	// 	ft_lstadd_back(&args->pids, ft_lstnew(p));
}

void	handle_pipe(int j, t_args *args, t_temp *p)
{
	char	**av;

	dup2(p->fdin, 0);
	close(p->fdin);
	av = ft_split(args->argv[j], ' ');
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
	if (is_builtin(av[0]) == 1)
		handle_builtin(av, args);
	else
		handle_command(av, args);
}

int	pipex(t_args *args)
{
	t_temp	p;
	int		j;
	int		exit_status;
	int		status;

	exit_status = 0;
	setup_fds(&p);
	j = 0;
	while (j < args->p_count + 1)
		handle_pipe(j++, args, &p);
	restore_fds(&p);
	j = 0;
	while (j != -1)
		j = wait(&status);
	exit_status = WEXITSTATUS(status);
	return (exit_status);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char	**split;
// 	t_export	*list = NULL;
// 	int	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		split = ft_split(envp[i], '=');
// 		populate(&list, split);
// 		i++;
// 	}
// 	char	buf[] = "~this~is~astring~";
// 	char	*str = tilde_exp(buf, &list);
// 	printf("%s\n", str);
// }