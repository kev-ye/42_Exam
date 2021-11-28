/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaye <kaye@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 12:46:45 by kaye              #+#    #+#             */
/*   Updated: 2021/11/28 19:56:19 by kaye             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
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
fd_set		fdSet;
fd_set		tmpSet;
char		msgRecv[1024 + 1] = {0};

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
	if (-1 != sockFd) {
		close(sockFd);
		FD_CLR(sockFd, &fdSet);
	}
	__clean__();
	exit(EXIT_FAILURE);
}

int		__getCliId__(int const fd) {
	t_client	*tmp;

	tmp = g_client;
	while (tmp) {
		if (fd == tmp->fd)
			return tmp->id;
		tmp = tmp->next;
	}
	return -1;
}

void	__addCli__(t_client *cli) {
	t_client	*tmp = g_client;

	if (NULL == tmp)
		g_client = cli;
	else {
		while (NULL != tmp && NULL != tmp->next)
			tmp = tmp->next;
		tmp->next = cli;
	}
	FD_SET(cli->fd, &fdSet);
}

void	__deleteCli__(int fd) {
	t_client	*tmp = g_client;
	t_client	*toDelete = NULL;
	int			id = 0;

	if (NULL != g_client && g_client->fd == fd) {
		toDelete = g_client;
		id = toDelete->id;
		g_client = g_client->next;
		free(toDelete);
		toDelete = NULL;
		return ;
	}
	while (NULL != tmp && NULL != tmp->next && tmp->next->fd != fd)
		tmp = tmp->next;
	toDelete = tmp->next;
	id = toDelete->id;
	printf("toDelelte: %d\n", id);
	tmp->next = tmp->next->next;
	free(toDelete);
	toDelete = NULL;
}

void	__sendToAll__(int fd, char *msg) {
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

void	__connection__(void) {
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

	sprintf(msg, "server: client %d just arrived\n", cliToAdd->id);
	__sendToAll__(connfd, msg);
}

void	__disconnection__(int const fd) {
	char	*msg = NULL;
	
	msg = calloc(__intLen__(fd) + sizeof("server: client  just left\n"), sizeof(char));
	if (NULL == msg)
		__exit__();

	sprintf(msg, "server: client %d just left\n", __getCliId__(fd));
	__sendToAll__(fd, msg);

	close(fd);
	FD_CLR(fd, &fdSet);

	__deleteCli__(fd);
}

void	__sendMsg__(int const fd) {
	char	tmp[1024 + 1] = {0};
	char	*toSend = NULL;
	int		i = 0;
	int		j = 0;

	while ('\0' != msgRecv[i]) {
		tmp[j++] = msgRecv[i];
		if ('\n' == msgRecv[i++]) {
			toSend = calloc(__intLen__(fd) + sizeof("client  : ") + strlen(tmp), sizeof(char));
			if (NULL == toSend)
				__exit__();
			sprintf(toSend, "client %d: %s", __getCliId__(fd), tmp);
			__sendToAll__(fd, toSend);
			bzero(tmp, strlen(tmp));
			j = 0;
		}
	}
	bzero(msgRecv, strlen(msgRecv));
}

int		__maxFd__(void) {
	t_client	*tmp = g_client;
	int			max = sockFd;

	while (tmp) {
		if (tmp->fd > max)
			max = tmp->fd;
		tmp = tmp->next;
	}
	return max;
}

void	__createServ__(u_int16_t const port) {
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
	char const	*msg = "Wrong number of arguments\n";

	if (ac != 2) {
		write(STDERR_FILENO, msg, strlen(msg));
		exit(EXIT_FAILURE);
	}
	u_int16_t port = atoi(av[1]);
	__createServ__(port);

	FD_ZERO(&fdSet);
	FD_SET(sockFd, &fdSet);

	for (;;) {
		tmpSet = fdSet;
		int ready = select(__maxFd__() + 1, &tmpSet, NULL, NULL, NULL);
		if (read < 0)
			continue ;

		for (int i = 0; i < __maxFd__() + 1; i++) {
			if (FD_ISSET(i, &tmpSet)) {
				if (i == sockFd) {
					__connection__();
					break ;
				}
				else {
					bzero(msgRecv, sizeof(msgRecv));
					int	ret = recv(i, msgRecv, 1024, 0);
					if (ret <= 0) {
						__disconnection__(i);
						break ;
					}
					else {
						printf("msg: %s\n", msgRecv);
						__sendMsg__(i);
					}
				}
			}
		}
	}

	return 0;
}