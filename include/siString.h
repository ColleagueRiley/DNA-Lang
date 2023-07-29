/*
sili.h - a cross-platform software toolchain for modern C programming
===========================================================================

LICENSE
	- This software is dual-licensed to the public domain and under the MIT
	license (see the LICENSEs in the bottom of the file).

WARNING
	- This library is _slightly_ experimental and features may not work as
	expected.
	- This also means that many functions are not documented.

CREDITS
	- Ginger Bill's 'gb.h' for inspiring me to program the Sili Toolchain, as
	well as certain features taken from the library and implemented here.
*/

/*
ColleagueRiley
-
this is sili.h but I removed everything but siString
-
*/

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef SI_INCLUDE_SI_H
#define SI_INCLUDE_SI_H

/**
 * @typedef siString
 * @brief siString is a dynamic string type.
 */
typedef char* siString;

/**
 * @typedef siStringHeader
 * @brief siStringHeader is the header structure for siString.
 */
typedef struct siStringHeader {
	size_t len;
	size_t capacity;
	/* u8 type_size;
	u8 grow; */
} siStringHeader;

#define SI_STRING_HEADER(str) ((siStringHeader*)str - 1)
#define SI_STRING_DEFAULT_GROW 128

siString si_string_make(char* str);
siString si_string_make_len(char* str, size_t len);
siString si_string_make_reserve(size_t len);
size_t si_string_len(siString str);

void si_string_append(siString* str, char* other);
void si_string_append_len(siString* str, char* other, size_t other_len);

void si_string_clear(siString* str);

void si_string_free(siString str);
void si_string_make_space_for(siString* str, size_t add_len);
#endif
size_t si_cstr_len(char* str);
bool si_cstr_equal(char* str1, char* str2);

siString si_string_make_len(char* str, size_t len) {
	siString res_str = si_string_make_reserve(len);
	memcpy(res_str, str, len);

	siStringHeader* header = SI_STRING_HEADER(res_str);
	header->len = len;

	return res_str;
}

siString si_string_make_reserve(size_t len) {
	char* ptr = malloc(sizeof(siStringHeader) + (len + 1));

	siString res_str = (siString)ptr + sizeof(siStringHeader);
	res_str[len] = '\0';

	siStringHeader* header = SI_STRING_HEADER(res_str);
	header->len = 0;
	header->capacity = len;
	/*header->type_size = sizeof(char); */

	return res_str;
}

inline size_t si_string_len(siString str) {
	return SI_STRING_HEADER(str)->len;
}

inline size_t si_cstr_len(char* str) {
	char* s;
	for (s = str; *s; s++);

	return (s - str);
}

inline bool si_cstr_equal(char* str1, char* str2) {
    unsigned char* s; 
    unsigned char* s2 = str2;

    for (s = str1; *s; s++)
        if (*s != s2[s - (unsigned char*)str1])
            return 0;

    return (*s == s2[s - (unsigned char*)str1]);
}

inline void si_string_append(siString* str, char* other) {
	si_string_append_len(str, other, si_cstr_len(other));
}
void si_string_append_len(siString* str, char* other, size_t other_len) {
	siString cur_str = *str;

	siStringHeader* header = SI_STRING_HEADER(cur_str);
	size_t previous_len = header->len;
	header->len += other_len;

	if (header->capacity < header->len) {
        si_string_make_space_for(str, other_len);
		cur_str = *str;
		header = SI_STRING_HEADER(cur_str);
	}

	memcpy(cur_str + previous_len, other, other_len);
	cur_str[header->len] = '\0';
}
inline void si_string_free(siString str) {
	if (str == NULL) {
		return ;
	}
	free(SI_STRING_HEADER(str));
}
void si_string_make_space_for(siString* str, size_t add_len) {
	siStringHeader* header = SI_STRING_HEADER(*str);
	size_t old_size = sizeof(siStringHeader) + header->len + 1;
	size_t new_size = old_size + add_len;

	siStringHeader* new_header = (siStringHeader*)realloc(header, new_size);

    *str = (char*)new_header + sizeof(siStringHeader);
	new_header->capacity += add_len;
}


/*
------------------------------------------------------------------------------

This software is available under 2 licenses - you may choose the one you like.

------------------------------------------------------------------------------

ALTERNATIVE A - MIT License

Copyright (c) 2023 EimaMei

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

------------------------------------------------------------------------------

ALTERNATIVE B - Public Domain (www.unlicense.org)

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------------------------------------------
*/