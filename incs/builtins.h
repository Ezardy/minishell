/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazari <mamazari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:29:25 by mamazari          #+#    #+#             */
/*   Updated: 2024/05/15 19:08:24 by mamazari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_content
{
	char				*val;
	char				*key;
	struct s_content	*next;
}	t_content;

typedef struct s_export
{
	struct s_content	*pair;
	struct s_export		*next;
}	t_export;

typedef struct	s_temp
{
	int	tempin;
	int	tempout;
	int	fdout;
	int	fdin;
	int	*fd;
}	t_temp;

t_content	*ft_content_new(char *key, char *val);
t_export	*my_export(t_export *l, char **envp, char *s);
void		print_list(t_export **list);
void		ft_swap(t_export **l1, t_export **l2);
void		append(char *s, char **split, t_export **l);
void		populate(t_export **l, char **split);
void		sort_list(t_export **l);
char		*get_val(char *s);
int			append_list(char *s, char **split, t_export *l1, t_export *l2);
int			append_last(char *s, char **split, t_export *last);
int			append_first(char *s, char **split, t_export **first);
int			is_inside(char *s, char *key, t_export **l);
int			is_builtin(char *str);

#endif