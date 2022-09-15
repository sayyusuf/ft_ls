/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay <ysay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 15:24:26 by ysay              #+#    #+#             */
/*   Updated: 2022/09/15 17:09:26 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_ls	*ls_dup(const t_ls *ls, char *str)
{
	t_ls	*ret;

	ret = malloc(sizeof(t_ls));
	*ret = *ls;
	ret->name = ft_strdup(str);
	return (ret);
}

char	*get_attr(char *path)
{
	ssize_t	bufflen;
	char	*key;
	char	*buff;

	bufflen = listxattr(path, NULL, 0);
	if (bufflen < 1)
		return (ft_strdup(""));
	key = malloc(bufflen + 1);
	bufflen = listxattr(path, key, bufflen);
	key[bufflen] = '\0';
	bufflen = getxattr(path, key, NULL, 0);
	if (bufflen < 1)
	{
		free(key);
		return (ft_strdup(""));
	}
	buff = malloc(bufflen + 1);
	bufflen = getxattr(path, key, buff, bufflen);
	buff[bufflen] = '\0';
	free(key);
	return (buff);
}

char	*get_time(time_t time)
{
	char	**s1;
	char	*s;
	char	*tmp;
	size_t	i;

	s = ft_strtrim(ctime(&time), "\n");
	s1 = ft_split(s, ' ');
	free(s);
	tmp = ft_str3join(s1[1], " ", s1[2]);
	s = ft_str3join(tmp, " ", s1[3]);
	free(tmp);
	i = 0;
	while (s1[i])
		free(s1[i++]);
	free(s1);
	s[ft_strlen(s)- 3] = '\0';
	return (s);
}

void	set_info(t_info *info, char *path, struct stat st, char *name)
{
	info->name = ft_strdup(name);
	lstat(path, &st);
	info->attr = get_attr(path);
	info->perm = get_permission(path);
	info->link_count = ft_itoa(st.st_nlink);
	info->date = get_time(st.st_atime);
	info->size = ft_itoa(st.st_size);
	info->user = ft_strdup(getpwuid(st.st_uid)->pw_name);
	info->group = ft_strdup(getgrgid(st.st_gid)->gr_name);
}

//traRL
int	main(int ac, char *av[], char *env[])
{
	t_argv	*argv;
	t_ls	ls;
	t_argv	*targ;
	int		flag;
	size_t	i;

	if (ac == 0)
		return (0);
	flag = 0;
	ls = (t_ls){0};
	argv = argv_new((void **)&av[1], (void *(*)(void *)) ft_strdup);
	if (set_flags(argv, &ls) == -1)
		return (1);
	argv = sort_proc(&ls, argv);
	targ = set_target(argv, &ls);
	argv_destroy(argv, free);
		flag = targ->len;
	i = 0;
	while (i < targ->len)
	{
		open_dir(targ->array[i++], flag, !env);
		env = NULL;
	}
	argv_destroy(targ, (void (*)(void *))ls_destroy);
	return (0);
}
