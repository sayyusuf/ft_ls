#include <libft.h>
#include <argv.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>

typedef struct  s_ls
{
	size_t	block_s;
	char	*name;
	int		a_flag;
	int		t_flag;
	int		r_flag;
	int		br_flag;
	int		l_flag;
}	t_ls;

typedef	struct s_info
{
	char	*perm;
	char	*link_count;
	char	*user;
	char	*group;
	char	*size;
	char	*date;
	char	*atrr;
	char	*name;
}	t_info;

void	info_destrtoy(t_info *info)
{
		free(info->perm);
		free(info->link_count);
		free(info->user);
		free(info->group);
		free(info->size);
		free(info->date);
		free(info->name);
		free(info->atrr);
		free(info);
}


void	ls_destroy(t_ls *ls)
{
	free(ls->name);
	free(ls);
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

int set_flags(argv_t *argv, t_ls *ls)
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
			argv_dell_one(argv, i, free);
		}
		++i;
	}
	return (0);
}

t_ls	*ls_dup(const t_ls *ls,char *str)
{
	t_ls	*ret;

	ret = malloc(sizeof(t_ls));
	*ret = *ls;
	ret->name =  ft_strdup(str);
	return (ret);
}

argv_t	*set_target(argv_t *argv, t_ls *ls)
{
	size_t	i;
	argv_t	*ret;

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


argv_t	*sort_time(argv_t *list)
{
	struct stat		st;
	long int		time;
	size_t			index;
	size_t			i;
	argv_t			*nlist;

	nlist = argv_new(NULL, NULL);
	while (list->len)
	{
		i = 0;
		index = 0;
		time = 0;
		while (i < list->len)
		{
			stat(list->array[i], &st);
			if (st.st_mtime >= time)
			{
				time = st.st_mtime;
				index = i;
			}
			++i;
		}
		argv_push(nlist, list->array[index]);
		argv_dell_one(list, index, NULL);
	}
	argv_destroy(list, free);
	return (nlist);
}

void all_low(char *s)
{
	while (*s)
	{
		*s = ft_tolower(*s);
		s++;
	}
}

int cmp(char *s1, char *s2)
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
	all_low(t1);
	all_low(t2);
	if (*t1 == '.')
		i1 = 1;
	if (*t2 == '.')
		i2 = 1;
	ret = strcmp(&t1[i1], &t2[i2]);
	free(t1);
	free(t2);
	return (ret);
}

argv_t	*sort_name(argv_t *list)
{
	char			*name;
	size_t			index;
	size_t			i;
	argv_t			*nlist;

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
		argv_dell_one(list, index, NULL);
	}
	argv_destroy(list, free);
	return (nlist);
}

void	dell_hidden(argv_t *list)
{
	size_t	i;
	char	*s;
	i = 0;
	while (i < list->len)
	{
		s = list->array[i];
		if (*s == '.')
			argv_dell_one(list, i, free);
		else
			i++;
	}
}

argv_t	*sort_proc(t_ls *ls, argv_t *list)
{
	argv_t	*ret;

	ret = sort_name(list);
	if (ls->t_flag)
		ret = sort_time(ret);
	if (!ls->a_flag)
		dell_hidden(ret);
	if (ls->r_flag)
		argv_reverse(ret);
	return ret;
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

int	main(int ac, char *av[]);

void clear_redir(argv_t *list)
{
	size_t	i;

	i = 0;
	while (i < list->len)
		if (!ft_strcmp(".", (char *)list->array[i]) || !ft_strcmp("..", (char *)list->array[i]))
			argv_dell_one(list, i, free);
		else
			i++;
}

void clear_notdir(argv_t *list)
{
	size_t			i;
	struct	stat	st;
	i = 0;
	while (i < list->len)
	{
		lstat((char *) list->array[i], &st);
		if (!S_ISDIR(st.st_mode))
			argv_dell_one(list, i, free);
		else
			i++;
	}
}

int	open_br(t_ls *ls, argv_t *list)
{
	argv_t	*nlist;

	clear_redir(list);
	nlist = argv_new(NULL, NULL);
	while (list->len)
	{
		argv_push(nlist, ft_str3join(ls->name, "/", (char *)list->array[0]));
		argv_dell_one(list, 0, free);
	}
	clear_notdir(nlist);
	if (nlist->len)
		argv_push(nlist, make_flags(ls));
	if (nlist->len)
		argv_insert(nlist, 0, ft_strdup("ft_ls"));
	main(nlist->len, (char **)nlist->array);
	argv_destroy(nlist, free);
	return (0);
}


char	get_type(const char *str)
{
	struct stat st;

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
	return('-');
}

void bit_set_user(struct stat *st, char *s)
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
}

void bit_set_group(struct stat *st, char *s)
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
}

void bit_set_other(struct stat *st, char *s)
{
	if ((st->st_mode & S_IRWXO) & S_IROTH)
		s[0] = 'r';
	else
		s[0] = '-';
	if ((st->st_mode & S_IRWXO) & S_IWOTH)
		s[1] = 'w';
	else
		s[1] = '-';
	if ((st->st_mode & S_IRWXG) & S_IXOTH)
		s[2] = 'x';
	else
		s[2] = '-';
}

char	*get_permission(const char *name)
{
	char	*str;
	struct  stat st;

	lstat(name, &st);
	str = malloc(12);
	str[0] = get_type(name);
	bit_set_user(&st, &str[1]);
	bit_set_group(&st, &str[4]);
	bit_set_other(&st, &str[7]);
	str[10] = '\0';
	return (str);
}

char	*get_attr(char *path)
{
	ssize_t	bufflen;
	char	*key;
	char	*buff;

	bufflen = listxattr(path, NULL, 0);
	if (bufflen < 1)
		return(ft_strdup(""));
	key = malloc(bufflen + 1);
	bufflen = listxattr(path, key, bufflen);
	key[bufflen] = '\0';
	bufflen = getxattr(path, key, NULL, 0);
	if (bufflen < 1)
	{
		free(key);
		return(ft_strdup(""));
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
	return (s);
}

argv_t	*l_operation(const argv_t *list, t_ls *ls)
{
	argv_t		*nlist;
	t_info		*info;
	struct stat st;
	size_t		i;
	char		*path;

	i = 0;
	ls->block_s = 0;
	nlist = argv_new(NULL, NULL);
	while(i < list->len)
	{
		info = malloc(sizeof(t_info));
		*info = (t_info){0};
		info->name = ft_strdup((char *)list->array[i]);
		path = ft_str3join(ls->name, "/", info->name);
		lstat(path, &st);
		info->atrr = get_attr(path);
		info->perm = get_permission(path);
		info->link_count = ft_itoa(st.st_nlink);
		info->date = get_time(st.st_atime);
		info->size = ft_itoa(st.st_size);
		info->user = ft_strdup(getpwuid(st.st_uid)->pw_name);
		info->group = ft_strdup(getgrgid(st.st_gid)->gr_name);
		argv_push(nlist, info);
		++i;
		free(path);
		ls->block_s += st.st_blocks;
	}
	return (nlist);
}


void	print_list(const argv_t *list, t_ls *ls)
{
	size_t	i;
	t_info	*inf;
	argv_t	*nlist;
	i = 0;
	while (i < list->len && !ls->l_flag)
		ft_printf("%s\n", (char *)list->array[i++]);
	if (!ls->l_flag)
		return ;
	nlist = l_operation(list, ls);
	ft_printf("total %d\n", ls->block_s / 2);
	while (i < nlist->len)
	{
		inf = nlist->array[i++];		
		ft_printf("%s %s %s %s %s %s %s %s\n", inf->perm, inf->atrr, inf->link_count, inf->user, inf->group, inf->size, inf->date, inf->name);
	}
	argv_destroy(nlist, (void(*)(void*))info_destrtoy);
}

int open_dir(t_ls *ls, int flag)
{
	DIR				*dir;
	struct dirent	*dd;
	argv_t			*list;

	dir = opendir(ls->name);
	if (dir == NULL)
	{
		ft_printf("ft_ls: cannot open directory '%s': %s\n", ls->name, strerror(errno));
		return (2);
	}
	list = argv_new(NULL, NULL);
	while (dd = readdir(dir), dd != NULL)
		argv_push(list, ft_strdup(dd->d_name));
	list = sort_proc(ls, list);
	if (flag || ls->br_flag)
		ft_printf("\n");
	if (flag || ls->br_flag)
		ft_printf("%s:\n", ls->name);
	print_list(list, ls);
	closedir(dir);
	if (ls->br_flag)
		open_br(ls, list);
	argv_destroy(list, free);
	return (0);
}


//traRL
int	main(int ac, char *av[])
{
	argv_t	*argv;
	t_ls	ls;
	argv_t	*targ;
	int		flag;
	size_t	i;

	if (ac == 0)
		return (0);
	flag = 0;
	(void)ac;
	ls = (t_ls){0};
	argv = argv_new((void **)&av[1], (void *(*)(void *)) ft_strdup);
	if (set_flags(argv, &ls) == -1)
		return (1);
	targ = set_target(argv, &ls);
	argv_destroy(argv, free);
	if (targ->len > 1)
		flag = 1;
	i  = 0;
	while (i < targ->len)
		open_dir(targ->array[i++], flag);
	argv_destroy(targ, (void (*)(void *))ls_destroy);
	return (0);
} 