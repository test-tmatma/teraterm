﻿/*
 * Copyright (C) 2018- TeraTerm Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

typedef enum {
	FILE_CODE_NONE,
	FILE_CODE_UTF8,
	FILE_CODE_UTF16LE,
	FILE_CODE_UTF16BE,
	FILE_CODE_ACP,
} LoadFileCode;

char *LoadFileU8C(FILE *fp, size_t *_len, LoadFileCode *_code);
char *LoadFileU8A(const char *FileName, size_t *_len);
char *LoadFileU8W(const wchar_t *FileName, size_t *_len);
wchar_t *LoadFileWA(const char *FileName, size_t *_len);
char *LoadFileAA(const char *FileName, size_t *_len);
wchar_t *LoadFileWW(const wchar_t *FileName, size_t *_len);
unsigned char *LoadFileBinary(const wchar_t *FileName, size_t *_len);

#ifdef __cplusplus
}
#endif//__cplusplus
