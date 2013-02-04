/*
 * airsock.h
 * libairplay: Airplay implementation
 *
 * Created by Aaron Blakely on 2/3/13.
 * Copyright (C) 2013 Ephasic.  All rights reserved.
 *
 */

#ifndef LIBAIRPLAY_AIRSOCK_H
#define LIBAIRPLAY_AIRSOCK_H

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define MAX_SIZE	8192
#define PORT		5002
#define RETRY_COUNT	7
#define SERVLEN		80
#define RETRY_DELAY	1000
#define ERROR		-1
#define DEFAULT_IP	"127.0.0.1"
#define DEFAULT_UNIX	"/unix"

void delay(long ms, struct timeval *res);
int  init_listen_server(struct addrinfo **server_info, int port);
int  accept_clinet(int sock, struct addrinfo *server_info);
int  get_address(char *hostname, char *service, int family, int socktype, struct addrinfo **addrinfo);
int  create_client(struct addrinfo *server_info);
int  create_server(struct addrinfo *server_info);

#endif
