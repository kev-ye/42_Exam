/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaye <kaye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 12:46:45 by kaye              #+#    #+#             */
/*   Updated: 2021/11/28 15:57:52 by kaye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>z
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** @note #define for this exam is forbidden ... */

/** @brief preparation / initialize */

typedef struct s_client {
	int				id;
	int				fd;
	struct s_client	*next;
}	t_client;

t_client	*g_client = NULL;
int			sockFd = -1;
int			last_id = 0;

/** @brief funcion */

int	__intLen__(int i) {
	int len = 0;

	if (0 == i)
		return 1;
	while (i > 0) {
		i /= 10;
		++len;
	}
	return len;
}

void	__clean__(void) {
	t_client	*tmp = g_client;
	t_client	*toFree = NULL;

	while (NULL != tmp) {
		toFree = tmp;
		tmp = tmp->next;
		close(toFree->fd);
		free(toFree);
		toFree = NULL;
	}
}

void	__exit__(void) {
	char const *msg = "Fatal error\n";

	write(STDERR_FILENO, msg, strlen(msg));
	if (-1 != sockFd)
		close(sockFd);
	__clean__();
	exit(EXIT_FAILURE);
}

void	__addCli__(t_client *cli) {
	t_client	*tmp = g_client;

	if (NULL == tmp) {
		tmp = cli;
		return ;
	}
	while (NULL != tmp && NULL != tmp->next)
		tmp = tmp->next;
	tmp->next = cli;
}

void	_deleteCli(int fd) {
	t_client	*tmp = g_client;
	t_client	*toDelete = NULL;
	int			id = 0;

	if (NULL != tmp && tmp->fd == fd) {
		toDelete = tmp;
		id = toDelete->id;
		tmp = tmp->next;
		free(toDelete);
		toDelete = NULL;
		return id;
	}
	while (NULL != tmp && NULL != tmp->next && tmp->next->fd == fd)
		tmp = tmp->next;
	toDelete = tmp->next;
	id = toDelete->id;
	tmp->next = tmp->next->next;
	free(toDelete);
	toDelete = NULL;
	return id;
}

void	__sendToAll__(int fd, char const *msg) {
	t_client	*tmp = g_client;
	int			ret;

	while (tmp) {
		if (tmp->fd != fd) {
			ret = send(tmp->fd, msg, strlen(msg), 0);
			if (-1 == ret) {
				free(msg);
				msg = NULL;
				__exit__();
			}
		}
		tmp = tmp->next;
	}
	free(msg);
	msg = NULL;
}

int __connection__(void) {
	int					connfd;
	struct sockaddr_in	cli;
	socklen_t			len = sizeof(cli);
	t_client			*cliToAdd = NULL;
	char				*msg = NULL;

	connfd = accept(sockFd, (struct sockaddr *)&cli, &len);
	if (-1 == connfd)
		__exit__();
	
	cliToAdd = calloc(1, sizeof(t_client));
	if (NULL == cliToAdd)
		__exit__();

	cliToAdd->fd = connfd;
	cliToAdd->id = last_id++;
	__addCli__(cliToAdd);

	msg = calloc(__intLen__(cliToAdd->fd) + sizeof("server: client  just arrived\n"), sizeof(char));
	if (NULL == msg)
		__exit__();

	sprintf(msg, "server: client %d just arrived\n", cliToAdd->fd);
	return connfd;
}

int		__maxFd__(void) {
	t_client	*tmp = g_client;
	int			max = 0;

	while (tmp) {
		if (tmp->fd > max)
			max = tmp->fd;
		tmp = tmp->next;
	}
	return max;
}

void	__createServ__(u_int16_t port) {
	struct sockaddr_in	servaddr;

	sockFd = socket(AF_INET, SOCK_STREAM, 0); 
	if (-1 == sockFd)
		__exit__();
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(port); 

	if (0 != (bind(sockFd, (const struct sockaddr *)&servaddr, sizeof(servaddr))))
		__exit__();
	if (0 != listen(sockFd, SOMAXCONN))
		__exit__();
}

/** @brief main */

int	main(int ac, char **av) {
	char const *msg = "Wrong number of arguments\n";

	if (ac != 2) {
		write(STDERR_FILENO, msg, strlen(msg));
		exit(EXIT_FAILURE);
	}
	u_int16_t port = atoi(av[1]);
	__createServ__(port);
	return 0;
}