/*
 * Copyright 2014 Girino Vey!
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  See COPYING for more details.
 */

#include <stdlib.h>
#include <stdio.h>
#include "aes/aes.h"
#include "aes_decode.h"
#include "kernel_as_array.h"

static const int IV_SIZE = 16;

void decrypt(const char* key, const char* file, char* out, size_t *size) {
	// extracts IV
	char iv[32];
	char cypher[*size];
	aes_context ctx;

	memcpy(iv, file, IV_SIZE);
	memcpy(cypher, file + IV_SIZE, (*size) - IV_SIZE);

	// decodes into plain
	aes_setkey_dec(&ctx, key, 256);
	aes_crypt_cbc(&ctx,
	                    AES_DECRYPT,
	                    (*size) - IV_SIZE,
	                    iv,
	                    cypher,
	                    out );

	// removes padding
	int padding = out[(*size) - IV_SIZE - 1];
	out[(*size)-32-padding] = 0;

	*size = (*size) - IV_SIZE - padding;

}

void pad(const char* in, char* out, int bs) {
	int i;
	int len_in = strlen(in);
	char pad_c = bs - (len_in % bs);
	for (i = 0; i < len_in; i++) {
		out[i] = in[i];
	}
	for (i = len_in; i < len_in + pad_c; i++) {
		out[i] = pad_c;
	}
	out[len_in + pad_c] = 0;
}

void aes_load_program(char* out, size_t *sizep) {
	char key[255];
	*sizep = FILE_AS_ARRAY_SIZE;
	pad(AES_KEY, key, 32);
	decrypt(key, FILE_AS_ARRAY, out, sizep);
}
