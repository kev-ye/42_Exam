/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaye <kaye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 17:18:05 by kaye              #+#    #+#             */
/*   Updated: 2021/07/18 16:52:04 by kaye             ###   ########.fr       */
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

#define SUCCESS 0
#define FAILURE 1

#define NO_FLAG 0
#define PIPE 1
#define MULTI 2

typedef struct s_microshell
{
    char    **args;
    size_t  len;
    int     flag;
}   t_microshell;

extern char **environ;
t_microshell microshell;

void    _b_zero(void *ptr, size_t size)
{
    while (size)
        ((unsigned char *)ptr)[--size] = 0;
}

void    *_calloc(size_t count, size_t size)
{
    void *ptr;

    ptr = (void *)malloc(count * size);
    if (ptr)
        _b_zero(ptr, count * size);
    return (ptr);
}

size_t    _strlen(char *s)
{
    char *tmp;

    tmp = s;
    while (*tmp)
        ++tmp;
    return (tmp - s);
}

char    *_strdup(char *s)
{
    char *new_s;
    int i;

    if (!s)
        return (NULL);
    i = _strlen(s);
    new_s = _calloc(i, sizeof(char));
    if (!new_s)
        return (NULL);
    i = 0;
    while (s[i])
    {
        new_s[i] = s[i];
        ++i;
    }
    new_s[i] = '\0';
    return (new_s);
}

size_t    _agrs_len(char **s)
{
    size_t i;

    i = 0;
    while (s && s[i] != NULL)
    {
        if ((s[i][0] == '|' || s[i][0] == ';') && _strlen(s[i]) == 1)
            break ;
        ++i;
    }
    return (i);
}

void    _putstr_fd(char *s, int fd)
{
    write(fd, s, _strlen(s));
}

void    _free_args(void)
{
    if (microshell.args)
    {
        for (size_t i = 0; microshell.args[i]; i++)
        {
            free(microshell.args[i]);
            microshell.args[i] = NULL;
        }
        free(microshell.args);
        microshell.args = NULL;
    }
}

void    __exit__(char *msg1, char *msg2, int fd, int ret)
{
    if (msg1)
        _putstr_fd(msg1, fd);
    if (msg2)
        _putstr_fd(msg2, fd);
    _free_args();
    exit(ret);
}

void    _cd(void)
{
    
}

void    _parser(char **av, int i)
{
    int             j;
    int             k;
    size_t          len;

    _b_zero(&microshell, sizeof(t_microshell));
    len = _agrs_len(av + i);
    microshell.args = _calloc(len + 1, sizeof(char *));
    if (!microshell.args)
        __exit__(E_FATAL, NULL, STDERR_FILENO, FAILURE);
    if (av[len + i] && av[len + i][0] == '|' && _strlen(av[len + i]) == 1)
        microshell.flag = PIPE;
    else if (av[len + i] && av[len + i][0] == ';' && _strlen(av[len + i]) == 1)
        microshell.flag = MULTI;
    else
        microshell.flag = NO_FLAG;
    microshell.len = len;
    j = i;
    k = 0;
    while (av[j] && k < len)
    {
        microshell.args[k] = _strdup(av[j]);
        ++k;
        ++j;
    }
}

void    _fork_exec(void)
{
    pid_t pid;

    if (!strcmp(microshell.args[0], "cd"))
        _cd();
    else
    {
        pid = fork();
        if (pid < 0)
            __exit__(E_FATAL, NULL, STDERR_FILENO, FAILURE);
        else if (pid == 0)
        {
            if (execve(microshell.args[0], microshell.args, environ) == -1)
                __exit__(NULL, NULL, STDERR_FILENO, FAILURE);
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }
}

void    _exec(char **av)
{
    size_t i;
    int j;

    i = 1;
    while (av[i])
    {
        _parser(av, i);
        if (microshell.flag == NO_FLAG || microshell.flag == MULTI)
            _fork_exec();
        if (microshell.flag == NO_FLAG)
            i += microshell.len;
        else
            i += microshell.len + 1;
        _free_args();
    }
}


int main(int ac, char **av)
{
    if (ac <= 1)
        return (0);
    _exec(av);
    return (0);
}