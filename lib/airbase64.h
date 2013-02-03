/*
 * airbase64.h
 * libairplay
 *
 * Created by Aaron Blakely on 2/3/13.
 * Copyright (C) 2013 Ephasic.  All rights reserved.
 *
 */

#ifndef LIBAIRPLAY_AIRBASE64_H
#define LIBAIRPLAY_AIRBASE64_H

/* NOTE: All calls to the following functions need to be freed. */
char *airbase64_decode(unsigned char *in, int length, int actual_length);
char *airbase64_encode(unsigned char *input, int lenght);

#endif
