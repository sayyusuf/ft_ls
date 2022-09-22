/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay <ysay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 22:56:02 by ysay              #+#    #+#             */
/*   Updated: 2022/09/15 17:11:29 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int	open_br(t_ls *ls, t_argv *list)
{
	t_argv	*nlist;

	clear_redir(list);
	nlist = argv_new(NULL, NULL);
	while (list->len)
	{
		argv_push(nlist, ft_str3join(ls->name, "/", (char *)list->array[0]));
		argv_del_one(list, 0, free);
	}
	clear_notdir(nlist);
	if (nlist->len)
		argv_push(nlist, make_flags(ls));
	if (nlist->len)
		argv_insert(nlist, 0, ft_strdup("ft_ls"));
	main(nlist->len, (char **)nlist->array, NULL);
	argv_destroy(nlist, free);
	return (0);
}

void	look_nl(t_argv *list, t_ls *ls, int flag, int flag2)
{
	if ((ls->br_flag) && flag2)
		ft_printf("\n");
	if ((flag || ls->br_flag) && list->len > 3)
		ft_printf("%s:\n", ls->name);
}

int	error_masg(t_ls *ls)
{
	if (errno == 13)
		ft_printf("ft_ls: cannot open directory '%s': %s\n",
			ls->name, strerror(errno));
	if (errno == 2)
		ft_printf("ft_ls: cannot access '%s': %s\n",
			ls->name, strerror(errno));
	return (2);
}

int	open_dir(t_ls *ls, int flag, int flag2, size_t len)
{
	DIR				*dir;
	struct dirent	*dd;
	t_argv			*list;

	dir = opendir(ls->name);
	if (dir == NULL)
		return (error_masg(ls));
	list = argv_new(NULL, NULL);
	dd = readdir(dir);
	while (dd != NULL)
	{
		argv_push(list, ft_strdup(dd->d_name));
		dd = readdir(dir);
	}
	list = sort_proc(ls, list);
	if (len > 1)
		look_nl(list, ls, flag, flag2);
	print_list(list, ls);
	closedir(dir);
	if (ls->br_flag)
		open_br(ls, list);
	argv_destroy(list, free);
	return (0);
}
