/*
 * airplay.c
 * libairplay: Airplay Implementation
 *
 * Written by Aaron Blakely on 2/3/2013.
 * Copyright 2013 (C) Ephasic.  All rights reserved.
 *
 */

#include <airsock.h>
#include <airplay.h>

#ifndef TRUE
#define TRUE (-1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

// Temporary information
int kCurrentLogLevel = LOG_INFO;
int bufferStartFill  = -1;

// Shairport has Win32 support, so
// I'll copy the macros.  I cannot garuntee
// libairplay will work on it though.  My focus is
// OS X and *nix

#ifdef _WIN32
#define DEVNULL "nul"
#else
#define DEVNULL "/dev/null"
#endif
