/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaye <kaye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 17:18:05 by kaye              #+#    #+#             */
/*   Updated: 2021/07/13 17:43:03 by kaye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define E_CD_ARG "error: cd: bad arguments\n"
#define E_CD_FAIL "error: cd: cannot change directory to"
#define E_FATAL "error: fatal\n"


typedef struct s_minishell
{
    char **args;
}   t_minishell;

t_minishell *minishell;

size_t    ft_strlen(char *s)
{
    char *tmp;

    tmp = s;
    while (*tmp)
        ++tmp;
    return (tmp - s);
}

size_t    ft_strslen(char **s)
{
    size_t i;

    i = 0;
    while (s && s[i] != NULL)
        ++i;
    return (i);
}

void    ft_putstr_fd(char *s, int fd)
{
    write(fd, s, ft_strlen(s));
}

void    __exit__(char *msg1, char *msg2, int fd)
{
    if (msg1)
        ft_putstr_fd(msg1, fd);
    if (msg2)
        ft_putstr_fd(msg2, fd);
    exit(1);
}

void    cd(char *av)
{
    
}

int main(int ac, char **av)
{
    if (ac <= 1)
        return (0);
    int i = chdir(av[1]);
    printf("[%d]\n", i);
    return (0);
}