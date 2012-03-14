/*
 *  Copyright (c) 2009 Public Software Group e. V., Berlin, Germany
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */


/*
 *  File name:    utf8proc.h
 *
 *  Description:
 *  Header files for libutf8proc, which is a mapping tool for UTF-8 strings
 *  with following features:
 *  - decomposing and composing of strings
 *  - replacing compatibility characters with their equivalents
 *  - stripping of "default ignorable characters"
 *    like SOFT-HYPHEN or ZERO-WIDTH-SPACE
 *  - folding of certain characters for string comparison
 *    (e.g. HYPHEN U+2010 and MINUS U+2212 to ASCII "-")
 *    (see "LUMP" option)
 *  - optional rejection of strings containing non-assigned code points
 *  - stripping of control characters
 *  - stripping of character marks (accents, etc.)
 *  - transformation of LF, CRLF, CR and NEL to line-feed (LF)
 *    or to the unicode chararacters for paragraph separation (PS)
 *    or line separation (LS).
 *  - unicode case folding (for case insensitive string comparisons)
 *  - rejection of illegal UTF-8 data
 *    (i.e. UTF-8 encoded UTF-16 surrogates)
 *  - support for korean hangul characters
 *  Unicode Version 5.0.0 is supported.
 */


#ifndef UTF8PROC_H
#define UTF8PROC_H


#include <stdlib.h>
#include <sys/types.h>
#ifdef _MSC_VER
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
#ifdef _WIN64
#define ssize_t __int64
#else
#define ssize_t int
#endif
typedef unsigned char bool;
enum {false, true};
#else
#include <stdbool.h>
#include <inttypes.h>
#endif
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SSIZE_MAX
#define SSIZE_MAX ((size_t)SIZE_MAX/2)
#endif

#define UTF8PROC_ERROR_NOMEM -1
#define UTF8PROC_ERROR_OVERFLOW -2
#define UTF8PROC_ERROR_INVALIDUTF8 -3
#define UTF8PROC_ERROR_NOTASSIGNED -4
#define UTF8PROC_ERROR_INVALIDOPTS -5
/*
 *  Error codes being returned by almost all functions:
 *  ERROR_NOMEM:       Memory could not be allocated.
 *  ERROR_OVERFLOW:    The given string is too long to be processed.
 *  ERROR_INVALIDUTF8: The given string is not a legal UTF-8 string.
 *  ERROR_NOTASSIGNED: The REJECTNA flag was set,
 *                     and an unassigned code point was found.
 *  ERROR_INVALIDOPTS: Invalid options have been used.
 */

extern const int8_t utf8proc_utf8class[256];

ssize_t utf8proc_iterate(const uint8_t *str, ssize_t strlen, int32_t *dst);
/*
 *  Reads a single char from the UTF-8 sequence being pointed to by 'str'.
 *  The maximum number of bytes read is 'strlen', unless 'strlen' is
 *  negative.
 *  If a valid unicode char could be read, it is stored in the variable
 *  being pointed to by 'dst', otherwise that variable will be set to -1.
 *  In case of success the number of bytes read is returned, otherwise a
 *  negative error code is returned.
 */

bool utf8proc_codepoint_valid(int32_t uc);
/*
 *  Returns 1, if the given unicode code-point is valid, otherwise 0.
 */

ssize_t utf8proc_encode_char(int32_t uc, uint8_t *dst);
/*
 *  Encodes the unicode char with the code point 'uc' as an UTF-8 string in
 *  the byte array being pointed to by 'dst'. This array has to be at least
 *  4 bytes long.
 *  In case of success the number of bytes written is returned,
 *  otherwise 0.
 *  This function does not check if 'uc' is a valid unicode code point.
 */

ssize_t utf8proc_charlen(const uint8_t *str, ssize_t str_len);
ssize_t utf8proc_strlen(const uint8_t *str, ssize_t len);

#ifdef __cplusplus
}
#endif

#endif

