/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazari <mamazari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 16:20:42 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/08 17:07:32 by mamazari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/minishell.h"

int	is_builtin(char *str)
{
	static char	*buf[8] = {"echo", "cd", "pwd", "export", \
		"unset", "env", "exit", NULL};
	int			i;
	int			ans;

	i = 0;
	ans = 0;
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

void	my_pwd(void)
{
	char	*buf;
	char	*res;

	buf = (char *) malloc(sizeof(char) * (PATH_MAX + 1));
	if (!buf)
		exit(0);
	res = getcwd(buf, PATH_MAX);
	res[PATH_MAX] = '\0';
	if (res != NULL)
	{
		ft_putstr_fd(res, 1);
		free(res);
	}
	else
		exit(0);
}

void	my_echo(char **strs)
{
	int	i;
	int	flag;

	flag = 0;
	i = 1;
	if (ft_strncmp("-n", strs[i], 2) == 0)
	{
		flag = 1;
		i++;
	}
	while (strs[i] != NULL)
	{
		printf("%s", strs[i]);
		if (strs[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (flag == 0)
		printf("\n");
}

int	contains(char *s1, char *s2)
{
	char	**split;
	int		i;
	int		ans;

	split = ft_split(s1, '\n');
	i = 0;
	ans = 0;
	while (split[i])
	{
		if (ft_strncmp(s2, split[i], ft_strlen(s2)) == 0)
		{
			ans = 1;
			break ;
		}
		i++;
	}
	return (ans);
}

void	print_list(t_list **list)
{
	t_list *temp = *list;
	while (temp != NULL)
	{
		printf("%s", (char*)temp->content);
		temp = temp->next;
	}
}

char	*my_env(char *str, char **envp, char *s)
{
	int		i;
	char	*s2;
	i = 0;
	if (str == NULL)
	{
		while (envp[i])
		{
			s2 = ft_strjoin(str, envp[i]);
			free(str);
			str = ft_strjoin(s2, "\n");
			free(s2);
			i++;
		}
	}
	if (contains(str, s) == 0)
	{
		s2 = ft_strjoin(str, s);
		free(str);
		str = ft_strjoin(s2, "\n");
		free(s2);
	}
	return (str);
}
int	split_len(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
		i++;
	return (i);
}

char	*add_brackets(char **split, char *env)
{
	char	*s1;
	
	if (env != NULL)
	{
		s1 = ft_strjoin("=\"", env);
		env = ft_strjoin(s1, "\"\n");
		free(s1);
	}
	else
	{
		env = (char *) malloc(sizeof(char) * 4);
		if (!env)
			env = NULL;
		else
		{
			env[0] = '\"';
			env[1] = '\"';
			env[2] = '\n';
			env[3] = '\0';
		}
		
	}
	return (env);
}

t_list	*my_export(t_list *l, char **envp, char *s)
{
	int		i;
	int		c;
	int		len;
	char	*str;
	char	*s1;
	char	*s2;
	t_list	*new;
	t_list	*l1;
	t_list	*l2;
	char	*temp;
	char	**split;
	
	i = 0;
	if (l == NULL)
	{
		while (envp[i] != NULL)
		{
			split = ft_split(envp[i], '=');
			len = split_len(split);
			s2 = getenv(split[0]);
			s2 = add_brackets(split, s2);
			s1 = ft_strjoin("declare -x ", split[0]);
			str = ft_strjoin(s1, s2);
			free(s1);
			new = ft_lstnew(str);
			ft_lstadd_back(&l, new);
			free_arr(split);
			free(s2);
			i++;
		}
		l1 = l;
		while (l1)
		{
			l2 = l1->next;
			while (l2)
			{
				if (ft_strncmp(l1->content, l2->content, \
					ft_strlen(l1->content) > ft_strlen(l2->content) ? ft_strlen(l2->content) : ft_strlen(l1->content)) > 1)
				{
					temp = l2->content;
					l2->content = l1->content;
					l1->content = temp;
				}
				l2 = l2->next;
			}
			l1 = l1->next;
		}
	}
	// if (ft_strlen(s) > 1)
	// {
	// 	l1 = l;
	// 	if (ft_strncmp(s, l1->content, s))
	// 	while (l1)
	// 	{

	// 	}
	// }
	print_list(&l);
	return (l);
}

// int	main2(int argc, char **argv, char **envp)
// {
// 	t_list	*list = NULL;

// 	list = my_export(list, envp, "");
// 	ft_lstclear(&list, free);
// 	return (0);
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	main2(argc, argv, envp);
// 	system("leaks minishell");
// 	return (0);
// }

// typedef struct s_simple_cmd
// {
// 	char	*command;
// 	char	*args;
// }				t_simple_cmd;

// typedef struct s_cmd
// {
// 	int				cmd_count;
// 	t_simple_cmd	*simple_cmds;
// 	char			*outfile;
// 	char			*infile;
// }				t_cmd;

// int	command_count(char **cmd_line)
// {
// 	int	i;
// 	int	count;

// 	i = 0;
// 	count = 0;
// 	while (cmd_line[i])
// 	{
// 		if (access(cmd_line[i], F_OK | R_OK) == 0)
// 			count++;
// 		i++;
// 	}
// 	return (count);
// }

// int	type(char *str)
// {
// 	int	type;

// 	type = 0; // 0 for word
// 	if (ft_strncmp(str, "<", 1) == 0)
// 		type = 1; // 1 for <
// 	else if (ft_strncmp(str, ">", 1) == 0)
// 		type = 2; // 2 for >
// 	else if (ft_strncmp(str, "|", 1) == 0)
// 		type = 3; // 3 for |
// 	return (type);
// }
