/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay <ysay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 22:44:25 by ysay              #+#    #+#             */
/*   Updated: 2022/09/22 16:23:42 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_argv	*sort_time(t_argv *list)
{
	struct stat		st;
	struct timeval	t;
	size_t			index;
	size_t			i;
	t_argv			*nlist;

	nlist = argv_new(NULL, NULL);
	while (list->len)
	{
		i = 0;
		index = 0;
		t = (struct timeval){0};
		while (i < list->len)
		{
			lstat(list->array[i++], &st);
			if (st.st_mtime > t.tv_sec || (st.st_mtime == t.tv_sec
					&& st.st_mtimespec.tv_nsec / 1000 > t.tv_usec))
			{
				t.tv_sec = st.st_mtimespec.tv_sec;
				t.tv_usec = st.st_mtimespec.tv_sec / 1000;
				index = i - 1;
			}
		}
		argv_push(nlist, list->array[index]);
		argv_del_one(list, index, NULL);
	}
	return (nlist);
}

int	cmp(char *s1, char *s2)
{
	char	*t1;
	char	*t2;
	int		ret;
	int		i1;
	int		i2;

	i1 = 0;
	i2 = 0;
	t1 = ft_strdup(s1);
	t2 = ft_strdup(s2);
	ret = strcmp(&t1[i1], &t2[i2]);
	free(t1);
	free(t2);
	return (ret);
}

t_argv	*sort_name(t_argv *list)
{
	char			*name;
	size_t			index;
	size_t			i;
	t_argv			*nlist;

	nlist = argv_new(NULL, NULL);
	while (list->len)
	{
		i = 1;
		index = 0;
		name = list->array[0];
		while (i < list->len)
		{
			if (cmp(list->array[i], name) < 0)
			{
				name = list->array[i];
				index = i;
			}
			++i;
		}
		argv_push(nlist, list->array[index]);
		argv_del_one(list, index, NULL);
	}
	argv_destroy(list, free);
	return (nlist);
}

t_argv	*sort_proc(t_ls *ls, t_argv *list)
{
	t_argv	*ret;

	ret = sort_name(list);
	if (ls->t_flag)
	{
		list = sort_time(ret);
		argv_destroy(ret, free);
		ret = list;
	}
	if (!ls->a_flag)
		del_hidden(ret);
	if (ls->r_flag)
		argv_reverse(ret);
	return (ret);
}
