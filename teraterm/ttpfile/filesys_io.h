﻿/*
 * (C) 2020 TeraTerm Project
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

#include <sys/types.h>	// for struct utimbuf
#include <sys/stat.h>
#include <sys/utime.h>
#include <windows.h>	// for BOOL

typedef struct FileIO {
	// file I/O, Filename related functions
	BOOL (*OpenRead)(struct FileIO *fv, const char *filename);
	BOOL (*OpenWrite)(struct FileIO *fv, const char *filename);
	size_t (*ReadFile)(struct FileIO *fv, void *buf, size_t bytes);
	size_t (*WriteFile)(struct FileIO *fv, const void *buf, size_t bytes);
	void (*Close)(struct FileIO *fv);
	int (*Seek)(struct FileIO *fv, size_t offset);
	void (*FileSysDestroy)(struct FileIO *fv);
	//
	size_t (*GetFSize)(struct FileIO *fv, const char *filename);
	int (*utime)(struct FileIO *fv, const char *filename, struct _utimbuf* const _Time);
	BOOL (*SetFMtime)(struct FileIO *fv, const char *FName, DWORD mtime);
	int (*stat)(struct FileIO *fv, const char *filename, struct _stati64* _Stat);
	char *(*GetSendFilename)(struct FileIO *fv, const char *fullname, BOOL utf8, BOOL space, BOOL upper);
	char *(*GetRecieveFilename)(struct FileIO *fv, const char* filename, BOOL utf8, const char *path, BOOL unique);
	long (*GetFMtime)(struct FileIO *fv, const char *fullname);
	//
	BOOL (*SetFilenameEncodeUTF8)(struct FileIO *fv, BOOL utf8);
	//
	void *data;
} TFileIO;
