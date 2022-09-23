/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay  <akarahan@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 22:48:56 by ysay              #+#    #+#             */
/*   Updated: 2022/09/22 15:32:02 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char	get_type(const char *str)
{
	struct stat	st;

	lstat(str, &st);
	if ((st.st_mode & S_IFMT) == S_IFBLK)
		return ('b');
	if ((st.st_mode & S_IFMT) == S_IFCHR)
		return ('c');
	if ((st.st_mode & S_IFMT) == S_IFDIR)
		return ('d');
	if ((st.st_mode & S_IFMT) == S_IFIFO)
		return ('p');
	if ((st.st_mode & S_IFMT) == S_IFLNK)
		return ('l');
	if ((st.st_mode & S_IFMT) == S_IFSOCK)
		return ('l');
	return ('-');
}

void	bit_set_user(struct stat *st, char *s)
{
	if ((st->st_mode & S_IRWXU) & S_IRUSR)
		s[0] = 'r';
	else
		s[0] = '-';
	if ((st->st_mode & S_IRWXU) & S_IWUSR)
		s[1] = 'w';
	else
		s[1] = '-';
	if ((st->st_mode & S_IRWXU) & S_IXUSR)
		s[2] = 'x';
	else
		s[2] = '-';
	if ((st->st_mode & S_ISUID))
		s[2] = 'S';
	else
		s[2] = '-';
}

void	bit_set_group(struct stat *st, char *s)
{
	if ((st->st_mode & S_IRWXG) & S_IRGRP)
		s[0] = 'r';
	else
		s[0] = '-';
	if ((st->st_mode & S_IRWXG) & S_IWGRP)
		s[1] = 'w';
	else
		s[1] = '-';
	if ((st->st_mode & S_IRWXG) & S_IXGRP)
		s[2] = 'x';
	else
		s[2] = '-';
	if ((st->st_mode & S_ISGID))
		s[2] = 'S';
}

void	bit_set_other(struct stat *st, char *s)
{
	if ((st->st_mode & S_IRWXO) & S_IROTH)
		s[0] = 'r';
	else
		s[0] = '-';
	if ((st->st_mode & S_IRWXO) & S_IWOTH)
		s[1] = 'w';
	else
		s[1] = '-';
	if ((st->st_mode & S_IRWXO) & S_IXOTH)
		s[2] = 'x';
	else
		s[2] = '-';
	if ((st->st_mode & S_ISTXT))
		s[2] = 'T';
}

char	*get_permission(const char *name)
{
	char		*str;
	struct stat	st;

	lstat(name, &st);
	str = malloc(12);
	str[0] = get_type(name);
	bit_set_user(&st, &str[1]);
	bit_set_group(&st, &str[4]);
	bit_set_other(&st, &str[7]);
	str[10] = '\0';
	return (str);
}
