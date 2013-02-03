/*
 * airbase64.h: libairplay base64 encoder and decoder based on OpenSSL
 * libairplay:  Airplay implementation
 *
 * Written by Aaron Blakely on 2/3/13.
 * Copyright (C) 2013 Ephasic.  All rights reserved.
 *
 */

#include <stdio.h>
#include <airbase64.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <oepnssl/bio.h>
#include <openssl/buffer.h>

static int get_corrected_encodesize(int size)
{
	if (size % 4 == 0)
	{
		return size;
	}
	else if ((size + 1) % 4 == 0)
	{
		return size + 1;
	}
	else if ((size + 2) % 4 == 0)
	{
		return size + 2;
	}
	else
	{
		// Invalid encoded data, no other cases are possible.
		printf("airbase64: Uncoverable error: base64 values are incorrectly encoded.\n");
		return size;
	}
}

char *airbase64_decode(unsigned char *input, int len, int actual_len)
{
	BIO *b64, *bmem;
	unsigned char *in	= input;
	int length		= get_corrected_encodedsize(len);

	if (len != length)
	{
		input		= malloc(length * sizeof(unsigned char));

		memset(in, 0, length);
		memcpy(in, input, len);
		memset(in+len, '=', length-len);
	}

	char *buffer = (char *)malloc(length);
	memset(buffer, 0, length);

	b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

	bmem = BIO_new_mem_buf(input, length);
	BIO_set_flags(bmem, BIO_FLAGS_BASE64_NO_NL);
	bmem = BIO_push(b64, bmem);

	BIO_set_flags(bmem, BIO_FLAGS_BASE64_NO_NL);

	*actual_len = BIO_read(bmem, buffer, length);

	BIO_free_all(bmem);
	if (len != length)
		free(in);

	return buffer;
}

char *airbase64_encode(unsigned char *input, int length)
{
	BIO *bmem, *b64;
	BUF_MEM *bptr;

	b64 = BIO_new(BIO_f_base64());

	// This enables/disables NLs.
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	bmem = BIO_new(BIO_s_mem());

	b64 = BIO_push(b64, bmem);

	BIO_write(b64, input, length);
	(void)BIO_flush(b64);

	BIO_get_mem_ptr(b64, &bptr);

	char *buff = (char *)malloc(bptr->length);

	memcpy(buff, bptr->data, bptr->length - 1);
	buff[bptr->length-1] = 0;

	BIO_free_all(b64);
	return buf;
}
