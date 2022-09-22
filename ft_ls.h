/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay <ysay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 22:22:56 by ysay              #+#    #+#             */
/*   Updated: 2022/09/22 16:22:49 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# include <libft.h>
# include <argv.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <stdlib.h>
# include <time.h>
# include <pwd.h>
# include <grp.h>
# include <sys/xattr.h>

typedef struct s_ls
{
	size_t	block_s;
	char	*name;
	int		a_flag;
	int		t_flag;
	int		r_flag;
	int		br_flag;
	int		l_flag;
}	t_ls;

typedef struct s_info
{
	char	*perm;
	char	*link_count;
	char	*user;
	char	*group;
	char	*size;
	char	*date;
	char	*attr;
	char	*name;
}	t_info;

int		main(int ac, char *av[], char *env[]);

int		set_flags(t_argv *argv, t_ls *ls);
int		init_flags(char *target, t_ls *ls);
t_argv	*set_target(t_argv *argv, t_ls *ls);
char	*make_flags(t_ls *ls);
char	*ft_str3join(char *s1, char *s2, char *s3);

void	info_destrtoy(t_info *info);
void	ls_destroy(t_ls *ls);
void	del_hidden(t_argv *list);
void	clear_redir(t_argv *list);
void	clear_notdir(t_argv *list);

t_argv	*sort_time(t_argv *list);
int		cmp(char *s1, char *s2);
t_argv	*sort_name(t_argv *list);
t_argv	*sort_proc(t_ls *ls, t_argv *list);

char	get_type(const char *str);
void	bit_set_user(struct stat *st, char *s);
void	bit_set_group(struct stat *st, char *s);
void	bit_set_other(struct stat *st, char *s);
char	*get_permission(const char *name);

t_argv	*l_operation(const t_argv *list, t_ls *ls);
void	look_algn(size_t *alg, t_argv *list);
char	*algn_str(size_t len, char *str, int back_flag);
void	do_algn(size_t *alg, t_argv *list);
void	print_list(const t_argv *list, t_ls *ls);

int		open_br(t_ls *ls, t_argv *list);
int		open_dir(t_ls *ls, int flag, int flag2, size_t len);

t_ls	*ls_dup(const t_ls *ls, char *str);
char	*get_attr(char *path);
char	*get_time(time_t time);
void	set_info(t_info *info, char *path, struct stat st, char *name);

#endif