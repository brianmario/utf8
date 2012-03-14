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
 *  This library contains derived data from a modified version of the
 *  Unicode data files.
 *
 *  The original data files are available at
 *  http://www.unicode.org/Public/UNIDATA/
 *
 *  Please notice the copyright statement in the file "utf8proc_data.c".
 */


/*
 *  File name:    utf8proc.c
 *
 *  Description:
 *  Implementation of libutf8proc.
 */


#include "utf8proc.h"

const int8_t utf8proc_utf8class[256] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0 };

ssize_t utf8proc_strlen(const uint8_t *str, ssize_t str_len)
{
	ssize_t char_count = 0;

	while (str_len > 0) {
		ssize_t char_len = utf8proc_charlen(str, str_len);
		if (char_len < 0)
			return UTF8PROC_ERROR_INVALIDUTF8;

		str += char_len;
		str_len -= char_len;
		char_count++;
	}

	return char_count;
}

ssize_t utf8proc_charlen(const uint8_t *str, ssize_t str_len)
{
	ssize_t length, i;

	if (!str_len)
		return 0;

	length = utf8proc_utf8class[str[0]];

	if (!length)
		return UTF8PROC_ERROR_INVALIDUTF8;

	if (str_len >= 0 && length > str_len)
		return UTF8PROC_ERROR_INVALIDUTF8;

	for (i = 1; i < length; i++) {
		if ((str[i] & 0xC0) != 0x80)
			return UTF8PROC_ERROR_INVALIDUTF8;
	}

	return length;
}

ssize_t utf8proc_iterate(const uint8_t *str, ssize_t str_len, int32_t *dst)
{
	ssize_t length;
	int32_t uc = -1;

	*dst = -1;
	length = utf8proc_charlen(str, str_len);
	if (length < 0)
		return UTF8PROC_ERROR_INVALIDUTF8;

	switch (length) {
		case 1:
			uc = str[0];
			break;
		case 2:
			uc = ((str[0] & 0x1F) <<  6) + (str[1] & 0x3F);
			if (uc < 0x80) uc = -1;
			break;
		case 3:
			uc = ((str[0] & 0x0F) << 12) + ((str[1] & 0x3F) <<  6)
				+ (str[2] & 0x3F);
			if (uc < 0x800 || (uc >= 0xD800 && uc < 0xE000) ||
					(uc >= 0xFDD0 && uc < 0xFDF0)) uc = -1;
			break;
		case 4:
			uc = ((str[0] & 0x07) << 18) + ((str[1] & 0x3F) << 12)
				+ ((str[2] & 0x3F) <<  6) + (str[3] & 0x3F);
			if (uc < 0x10000 || uc >= 0x110000) uc = -1;
			break;
	}

	if (uc < 0 || ((uc & 0xFFFF) >= 0xFFFE))
		return UTF8PROC_ERROR_INVALIDUTF8;

	*dst = uc;
	return length;
}

bool utf8proc_codepoint_valid(int32_t uc)
{
	if (uc < 0 || uc >= 0x110000 ||
		((uc & 0xFFFF) >= 0xFFFE) || (uc >= 0xD800 && uc < 0xE000) ||
		(uc >= 0xFDD0 && uc < 0xFDF0))
		return false;

	return true;
}

ssize_t utf8proc_encode_char(int32_t uc, uint8_t *dst)
{
	if (uc < 0x00) {
		return 0;
	} else if (uc < 0x80) {
		dst[0] = uc;
		return 1;
	} else if (uc < 0x800) {
		dst[0] = 0xC0 + (uc >> 6);
		dst[1] = 0x80 + (uc & 0x3F);
		return 2;
	} else if (uc == 0xFFFF) {
		dst[0] = 0xFF;
		return 1;
	} else if (uc == 0xFFFE) {
		dst[0] = 0xFE;
		return 1;
	} else if (uc < 0x10000) {
		dst[0] = 0xE0 + (uc >> 12);
		dst[1] = 0x80 + ((uc >> 6) & 0x3F);
		dst[2] = 0x80 + (uc & 0x3F);
		return 3;
	} else if (uc < 0x110000) {
		dst[0] = 0xF0 + (uc >> 18);
		dst[1] = 0x80 + ((uc >> 12) & 0x3F);
		dst[2] = 0x80 + ((uc >> 6) & 0x3F);
		dst[3] = 0x80 + (uc & 0x3F);
		return 4;
	} else return 0;
}

