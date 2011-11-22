#ifndef UTF8_UTF8_H
#define UTF8_UTF8_H

int8_t utf8CharLen(unsigned char *in, size_t in_len);
int64_t utf8CharCount(unsigned char *in, size_t in_len);
int32_t utf8CharToCodepoint(unsigned char *in, size_t in_len);

#endif