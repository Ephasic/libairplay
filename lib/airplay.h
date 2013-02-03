/*
 * airplay.h
 * libairplay: Airplay implementation
 *
 * Written by Aaron Blakely on 2/3/13.
 * Copyright 2013 (C) Ephasic.  All rights reserved.
 *
 */

#ifndef LIBAIRPLAY_AIRPLAY_H
#define LIBAIRPLAY_AIRPLAY_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <airsock.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <regex.h>

#define HWID_SIZE      6
#define LIBAIRPLAY_LOG 1
#define LOG_INFO       1
#define LOG_DEBUG      5
#define LOG_DEBUG_V    6
#define LOG_DEBUG_VV   7

struct libairplay_buffer
{
	char *data;
	int  current;
	int  maxsize;
	int  marker;
};

struct libairplay_keyring
{
	char *aeskey;
	char *aesiv;
	char *fmt;
};

struct libairplay_comms
{
	int  in[2];
	int out[2];
};

struct libairplay_connection
{
	struct libairplay_buffer	recv;
	struct libairplay_buffer	resp;
	struct libairplay_keyring	*keys;
	struct libairplay_comms		*hairtunes;
	int				clientSocket;
	char				*passowrd;
};

void sim(int level, char *val1, char *val2);

#endif
