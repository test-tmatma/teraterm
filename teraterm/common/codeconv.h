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

#include <stdlib.h>	// for size_t
#include "ttcstd.h"

#ifdef __cplusplus
extern "C" {
#endif

// simple code convert
unsigned int CP932ToUTF32(unsigned short cp932);
unsigned short UTF32ToDecSp(unsigned int u32);
unsigned int MBCP_UTF32(unsigned short mb_code, int code_page);
unsigned short UTF32_CP932(unsigned int u32);

// 1char ToUTF32
size_t UTF8ToUTF32(const char *u8_ptr_, size_t u8_len, unsigned int *u32_);
size_t UTF16ToUTF32(const wchar_t *wstr_ptr, size_t wstr_len, unsigned int *u32);
size_t MBCPToUTF32(const char *mb_ptr, size_t mb_len, int code_page, unsigned int *u32);

// 1char UTF32To
size_t UTF32ToUTF16(unsigned int u32, wchar_t *wstr_ptr, size_t wstr_len);
size_t UTF32ToUTF8(unsigned int u32, char *u8_ptr, size_t u8_len);
size_t UTF32ToCP932(unsigned int u32, char *mb_ptr, size_t mb_len);
size_t UTF32ToMBCP(unsigned int u32, int code_page, char *mb_ptr, size_t mb_len);

// MultiByteToWideChar() wrappers
void WideCharToUTF8(const wchar_t *wstr_ptr, size_t *wstr_len, char *u8_ptr, size_t *u8_len);
void WideCharToCP932(const wchar_t *wstr_ptr, size_t *wstr_len, char *cp932_ptr, size_t *cp932_len);
void WideCharToUTF32(const wchar_t *wstr_ptr, size_t *wstr_len_,
					 char32_t *u32_ptr, size_t *u32_len_);
int UTF8ToWideChar(const char *u8_ptr, int u8_len, wchar_t *wstr_ptr, int wstr_len);
void WideCharToACP_t(const wchar_t *wstr_ptr, char *mb_ptr, size_t mb_len);
size_t ACPToWideChar_t(const char *str_ptr, wchar_t *wstr_ptr, size_t wstr_len);

// API wrappers
char *_WideCharToMultiByte(const wchar_t *wstr_ptr, size_t wstr_len, int code_page, size_t *mb_len_);
char32_t *_WideCharToUTF32(const wchar_t *wstr_ptr, size_t wstr_len, size_t *u32_len_);
wchar_t *_MultiByteToWideChar(const char *str_ptr, size_t str_len, int code_page, size_t *w_len_);

// UTF-16
int IsHighSurrogate(wchar_t u16);
int IsLowSurrogate(wchar_t u16);

// convinience funcs  (for windows api params)
char *ToCharA(const char *strA);
char *ToCharW(const wchar_t *strW);
char *ToCharU8(const char *strU8);
wchar_t *ToWcharA(const char *strA);
wchar_t *ToWcharW(const wchar_t *strW);
wchar_t *ToWcharU8(const char *strU8);
char *ToU8A(const char *strA);
char *ToU8W(const wchar_t *strW);
char32_t *ToU32W(const wchar_t *strW);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#if defined(__GNUC__) || (defined(_MSC_VER) && (_MSC_VER > 1910))
#define	MOVE_CONSTRUCTOR_ENABLE
#endif

class u8
{
public:
	u8();
	u8(const char *strA);
	u8(const char *strA, int code_page);
	u8(const wchar_t *strW);
	u8(const u8 &obj);
#if defined(MOVE_CONSTRUCTOR_ENABLE)
	u8(u8 &&obj) noexcept;
#endif
	~u8();
	u8& operator=(const char *strA);
	u8& operator=(const wchar_t *strW);
	u8& operator=(const u8 &obj);
#if defined(MOVE_CONSTRUCTOR_ENABLE)
	u8& operator=(u8 &&obj) noexcept;
#endif
	operator const char *() const;
	const char *cstr() const;
private:
	char *u8str_;
	void assign(const char *strA, int code_page);
	void assign(const wchar_t *strW);
	void copy(const u8 &obj);
	void move(u8 &obj);
};

class wc
{
public:
	wc();
	wc(const char *strA);
	wc(const char *strA, int code_page);
	wc(const wchar_t *strW);
	wc(const wc &obj);
#if defined(MOVE_CONSTRUCTOR_ENABLE)
	wc(wc &&obj) noexcept;
#endif
	~wc();
	wc& operator=(const char *strA);
	wc& operator=(const wchar_t *strW);
	wc& operator=(const wc &obj);
#if defined(MOVE_CONSTRUCTOR_ENABLE)
	wc& operator=(wc &&obj) noexcept;
#endif
	static wc fromUtf8(const char *strU8);
	operator const wchar_t *() const;
	const wchar_t *cstr() const;
	operator const char *() const;
//	const char *cstr() const;
private:
	wchar_t *tstr_;
	void assign(const char *strA, int code_page);
	void assign(const wchar_t *strW);
	void copy(const wc &obj);
	void move(wc &obj);
};
#endif
