/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay <ysay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 22:38:37 by ysay              #+#    #+#             */
/*   Updated: 2022/09/15 12:46:04 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	info_destrtoy(t_info *info)
{
	free(info->perm);
	free(info->link_count);
	free(info->user);
	free(info->group);
	free(info->size);
	free(info->date);
	free(info->name);
	free(info->attr);
	free(info);
}

void	ls_destroy(t_ls *ls)
{
	free(ls->name);
	free(ls);
}

void	del_hidden(argv_t *list)
{
	size_t	i;
	char	*s;

	i = 0;
	while (i < list->len)
	{
		s = list->array[i];
		if (*s == '.' && *(s + 1) != '/')
			argv_del_one(list, i, free);
		else
			i++;
	}
}

void	clear_redir(argv_t *list)
{
	size_t	i;

	i = 0;
	while (i < list->len)
		if (!ft_strcmp(".", (char *)list->array[i])
			|| !ft_strcmp("..", (char *)list->array[i]))
			argv_del_one(list, i, free);
	else
			i++;
}

void	clear_notdir(argv_t *list)
{
	size_t			i;
	struct stat		st;

	i = 0;
	while (i < list->len)
	{
		lstat((char *) list->array[i], &st);
		if (!S_ISDIR(st.st_mode))
			argv_del_one(list, i, free);
		else
			i++;
	}
}
