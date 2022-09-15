/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay <ysay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 22:31:16 by ysay              #+#    #+#             */
/*   Updated: 2022/09/15 17:09:26 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int	set_flags(t_argv *argv, t_ls *ls)
{
	size_t	i;
	char	*str;

	i = 0;
	while (i < argv->len)
	{
		str = argv->array[i];
		if (*str == '-')
		{
			if (!init_flags(&str[1], ls))
				return (-1);
			argv_del_one(argv, i, free);
		}
		else
			++i;
	}
	return (0);
}

int	init_flags(char *target, t_ls *ls)
{
	if (*target == 0)
		return (0);
	while (*target)
	{
		if (*target == 'a')
			ls->a_flag = 1;
		else if (*target == 't')
			ls->t_flag = 1;
		else if (*target == 'r')
			ls->r_flag = 1;
		else if (*target == 'R')
			ls->br_flag = 1;
		else if (*target == 'l')
			ls->l_flag = 1;
		else
		{
			ft_printf("ls: invalid option -- '%c'\n", *target);
			ft_printf("Try 'ls --help' for more information.\n");
			return (0);
		}
		++target;
	}	
	return (1);
}

t_argv	*set_target(t_argv *argv, t_ls *ls)
{
	size_t	i;
	t_argv	*ret;

	i = 0;
	ret = argv_new(NULL, NULL);
	if (argv->len == 0)
		argv_push(ret, ls_dup(ls, "."));
	while (i < argv->len)
	{
		argv_push(ret, ls_dup(ls, argv->array[i]));
		++i;
	}
	return (ret);
}

char	*make_flags(t_ls *ls)
{
	char	*str;
	int		i;

	i = 0;
	str = malloc(sizeof(char) * 8);
	str[i++] = '-';
	if (ls->t_flag)
		str[i++] = 't';
	if (ls->a_flag)
		str[i++] = 'a';
	if (ls->r_flag)
		str[i++] = 'r';
	if (ls->br_flag)
		str[i++] = 'R';
	if (ls->l_flag)
		str[i++] = 'l';
	str[i] = '\0';
	if (ft_strlen(str) < 2)
	{
		free(str);
		str = NULL;
	}
	return (str);
}

char	*ft_str3join(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*ret;

	tmp = ft_strjoin(s1, s2);
	ret = ft_strjoin(tmp, s3);
	free(tmp);
	return (ret);
}
