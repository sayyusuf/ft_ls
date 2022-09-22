/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay <ysay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 22:52:04 by ysay              #+#    #+#             */
/*   Updated: 2022/09/15 18:02:39 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_argv	*l_operation(const t_argv *list, t_ls *ls)
{
	t_argv		*nlist;
	t_info		*info;
	struct stat	st;
	size_t		i;
	char		*path;

	i = 0;
	nlist = argv_new(NULL, NULL);
	while (i < list->len)
	{
		info = malloc(sizeof(t_info));
		path = ft_str3join(ls->name, "/", (char *)list->array[i]);
		lstat(path, &st);
		set_info(info, path, st, (char *)list->array[i]);
		argv_push(nlist, info);
		free(path);
		ls->block_s += st.st_blocks;
		++i;
	}
	return (nlist);
}

void	look_algn(size_t *alg, t_argv *list)
{
	size_t	i;
	t_info	*inf;

	i = 0;
	while (i < list->len)
	{
		inf = list->array[i];
		if (alg[0] < ft_strlen(inf->perm))
			alg[0] = ft_strlen(inf->perm);
		if (alg[1] < ft_strlen(inf->attr))
			alg[1] = ft_strlen(inf->attr);
		if (alg[2] < ft_strlen(inf->user))
			alg[2] = ft_strlen(inf->user);
		if (alg[3] < ft_strlen(inf->group))
			alg[3] = ft_strlen(inf->group);
		if (alg[4] < ft_strlen(inf->size))
			alg[4] = ft_strlen(inf->size);
		if (alg[5] < ft_strlen(inf->date))
			alg[5] = ft_strlen(inf->date);
		if (alg[6] < ft_strlen(inf->link_count))
			alg[6] = ft_strlen(inf->link_count);
		++i;
	}
}

char	*algn_str(size_t len, char *str, int back_flag)
{
	char	*ret;
	size_t	index;

	index = len - ft_strlen(str);
	ret = malloc(len + 1);
	if (back_flag)
	{
		ft_memcpy(ret, str, ft_strlen(str));
		ft_memset(&ret[ft_strlen(str)], ' ', index);
	}
	else
	{
		ft_memset(ret, ' ', index);
		ft_memcpy(&ret[index], str, ft_strlen(str));
	}
	ret[len] = '\0';
	free(str);
	return (ret);
}

void	do_algn(size_t *alg, t_argv *list)
{
	size_t	i;
	t_info	*inf;

	i = 0;
	while (i < list->len)
	{
		inf = list->array[i];
		inf->perm = algn_str(alg[0], inf->perm, 0);
		inf->attr = algn_str(alg[1], inf->attr, 1);
		inf->user = algn_str(alg[2], inf->user, 0);
		inf->group = algn_str(alg[3], inf->group, 0);
		inf->size = algn_str(alg[4], inf->size, 0);
		inf->date = algn_str(alg[5], inf->date, 0);
		inf->link_count = algn_str(alg[6], inf->link_count, 0);
		++i;
	}
}

void	print_list(const t_argv *list, t_ls *ls)
{
	size_t	i;
	t_info	*inf;
	t_argv	*nlist;
	size_t	*algn;

	algn = (size_t [7]){0};
	i = 0;
	while (i < list->len && !ls->l_flag)
		ft_printf("%s ", (char *)list->array[i++]);
	if (!ls->l_flag)
		ft_printf("\n");
	if (!ls->l_flag)
		return ;
	nlist = l_operation(list, ls);
	look_algn(algn, nlist);
	do_algn(algn, nlist);
	ft_printf("total %d\n", ls->block_s);
	while (i < nlist->len)
	{
		inf = nlist->array[i++];
		ft_printf("%s%s  %s %s  %s  %s %s %s\n", inf->perm, inf->attr,
			inf->link_count, inf->user, inf->group,
			inf->size, inf->date, inf->name);
	}
	argv_destroy(nlist, (void (*)(void *))info_destrtoy);
}
