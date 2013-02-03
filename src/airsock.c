/*
 * airsock.c: Airplay Socket Library
 * libairplay - Implementation of Airplay.
 *
 * Written by Aaron Blakely on 2/3/13.
 * Copyright (C) 2013 Ephasic.  All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <airsock.h>

int _internal_socket_constructor(struct addrinfo *addrInfo)
{
	int tmpSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, 0);

#ifdef AF_INET6
	if ((tmpSocket == -1) && (addrInfo->ai_family == AF_INET6) && (errno = EAFNOSUPPORT))
	{
		perror("airsock: Failed to create IPv6 socket, attempting to create IPv4.");
		addrInfo->ai_family = 0;

		tmpSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, 0);
	}
#endif

	return tmpSocket;
}

int create_client(struct addrinfo *server_info)
{
	int tmpSocketDescriptor = -1;
	int tmpRetryCount	=  0;

	while (tmpRetryCount++ < RETRY_COUNT)
	{
		tmpSocketDescriptor = _internal_socket_constructor(server_info);
		if (tmpSocketDescriptor < 0 && tmpRetryCount >= RETRY_COUNT)
		{
			perror("airsock: Couldn't create socket.");
			return ERROR;
		}

		if (connect(tmpSocketDescriptor, server_info->ai_addr, server_info->ai_addrlen) >= 0)
		{
			return tmpSocketDescriptor;
		}

		else
		{
			close(tmpSocketDescriptor);

			perror("airsock: Couldn't connect to server.");
			struct timeval tmpRes;

			delay(RETRY_DELAY, &tmpRes);
		}
	}

	printf("airsock: %d retry attempts exceeded.", RETRY_COUNT);
	return ERROR;
}

int get_address(char *hostname, char *service, int family, int socktype, struct addrinfo **addrinfo)
{
	int error = 0;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family		= family;
	hints.ai_socktype	= socktype;

	if (hostname == NULL)
		hints.ai_flags	= AI_PASSIVE;

	error = getaddrinfo(hostname, service, &hints, addrinfo);

	if (error != 0)
		printf("airsock: Error getting address information.\n");

	return error;
}

int create_server(struct addrinfo *server_info)
{
	int sock = _internal_socket_constructor(server_info);
	if (sock < 0)
	{
		perror("airsock: Couldn't create socket server.");
		return ERROR;
	}

	int enable = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

	if (bind(sock, server_info->ai_addr, server_info->ai_addrlen) < 0)
	{
		close(sock);

		perror("airsock: Unable to bind socket.");
		return ERROR;
	}

	if (listen(sock, 5) < 0)
	{
		close(sock);

		perror("airsock: Unable to listen on the server socket.");
		return ERROR;
	}

	return sock;
}

int accept_client(int sock, struct addrinfo *server_info)
{
	int tmpAccept = accept(sock, server_info->ai_addr, &server_info->ai_addrlen);

	if (tmpAccept < 0)
	{
		perror("airsock: Unable to accept client connection to server socket.");
		return ERROR;
	}

	return tmpAccept;
}

int init_listen_server(struct adrinfo **server_info, int port)
{
	char service[SERVLEN];
	sprintf(service, "%d", port);

	int family = AF_INET;
#ifdef AF_INET6
	family = AF_INET6;
#endif

	if (get_address(NULL, service, family, SOCK_STREAM, server_info))
	{
		return ERROR;
	}

	char tmpAddress[INET6_ADDRSTRLEN];
	int tmpSockDescriptor	= create_server(*server_info);
	socklen_t tmpSize	= INET6_ADDRSTRLEN;

	inet_ntop((*server_info)->ai_family, (*server_info)->ai_addr, tmpAddress, tmpSize);

	return tmpSockDescriptor;
}

void delay(long ms, struct timeval *res)
{
	res->tv_sec	= ms / 1000;
	res->tv_usec	= (ms - (res->tv_sec * 1000)) * 1000l

	selcet(0, NULL, NULL, NULL, res);
}
