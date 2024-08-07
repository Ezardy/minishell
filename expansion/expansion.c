/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanikin <zanikin@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 15:49:32 by zanikin           #+#    #+#             */
/*   Updated: 2024/07/02 21:03:27 by zanikin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "export/export.h"
#include "expansion_constants.h"

size_t		count_expanded_string(const char *str, t_export **ev,
				int error, int mask);
int			is_inside_quotes(char type, char *qtype);
char		*extract_ev(const char **pstr);
void		insert_number(char *dst, int n);
int			digits_count(int n);
char		*expand(const char *str, t_export **evlist, int error, int mask);

static void	expand_envvar(const char **pstr, char **exp_str,
				t_export **evlist, long m_err);
static void	insert_envvar_val(const char **pstr, char **exp_str,
				t_export **evlist);
static void	expand_loop(const char *str, char *exp_str, t_export **evlist,
				long m_err);

int	expand_list(char **strs, t_export **evlist, int error, int mask)
{
	int		expanded;
	size_t	i;
	char	*str;

	i = 0;
	expanded = strs != NULL;
	while (expanded && strs[i])
	{
		str = expand(strs[i], evlist, error, mask);
		if (str)
		{
			free(strs[i]);
			strs[i++] = str;
		}
		else
			expanded = 0;
	}
	return (expanded);
}

char	*expand(const char *str, t_export **evlist, int error, int mask)
{
	char	*exp_str;
	char	*home;
	size_t	exp_str_size;

	exp_str_size = count_expanded_string(str, evlist, error, mask);
	exp_str = (char *)malloc(sizeof(char) * (exp_str_size + 1));
	if (exp_str)
	{
		home = get_value_from_key(evlist, "HOME");
		if (!(mask & TILDA_MASK) && *str == '~' && home
			&& (!str[1] || str[1] == '/'))
		{
			expand_loop(str + 1, exp_str + ft_strlcpy(exp_str, home,
					ft_strlen(home) + 1), evlist, ((long)mask << 32) | error);
		}
		else
			expand_loop(str, exp_str, evlist, ((long)mask << 32) | error);
	}
	return (exp_str);
}

static void	expand_loop(const char *str, char *exp_str, t_export **evlist,
				long m_err)
{
	char	qtype;

	qtype = 0;
	while (*str)
	{
		if (!((m_err >> 32) & QUOTES_MASK) && (*str == '\'' || *str == '"'))
		{
			if (is_inside_quotes(*str, &qtype))
				*exp_str++ = *str;
		}
		else if (*str == '$' && (!qtype || qtype == '"'))
			expand_envvar(&str, &exp_str, evlist, m_err);
		else
			*exp_str++ = *str;
		str++;
	}
	*exp_str = '\0';
}

static void	expand_envvar(const char **pstr, char **exp_str,
					t_export **evlist, long m_err)
{
	char	type;

	if (!((m_err >> 32) & ENV_QUOTES_MASK)
		&& (pstr[0][1] == '\'' || pstr[0][1] == '"'))
	{
		type = (++pstr[0])[0];
		while ((++pstr[0])[0] != type)
			exp_str[0]++[0] = pstr[0][0];
	}
	else if (!((m_err >> 32) & ENV_ERR_MASK) && pstr[0][1] == '?')
	{
		pstr[0]++;
		insert_number(exp_str[0], (int)m_err);
		exp_str[0] += digits_count((int)m_err);
	}
	else if (((m_err >> 32) & ENV_EXP_MASK) || !pstr[0][1] || pstr[0][1] == ' '
				|| pstr[0][1] == '$')
		exp_str[0]++[0] = '$';
	else
		insert_envvar_val(pstr, exp_str, evlist);
}

static void	insert_envvar_val(const char **pstr, char **exp_str,
				t_export **evlist)
{
	char	*name;
	char	*env;

	name = extract_ev(pstr);
	env = get_value_from_key(evlist, name);
	free(name);
	if (env)
		exp_str[0] += ft_strlcpy(exp_str[0], env, ft_strlen(env) + 1);
}
