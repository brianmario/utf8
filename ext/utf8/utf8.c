#include <stdio.h>
#include <stdint.h>

#define CHECK_LEN if ((size_t)(in-start) >= in_len) return 0;

/*
 * Scans the current position of the buffer
 * returning the length of this UTF8 character
 */
inline int8_t utf8CharLen(unsigned char *in, size_t in_len) {
  if (in_len > 0) {
    unsigned char curChar, *start;

    start = in;
    curChar = in[0];
    in++;

    if (curChar <= 0x7f) {
      /* single byte */
      return 1;
    } else if ((curChar >> 5) == 0x6) {
      /* two byte */
      CHECK_LEN;
      curChar = in[0];
      in++;
      if ((curChar >> 6) == 0x2) return 2;
    } else if ((curChar >> 4) == 0x0e) {
      /* three byte */
      CHECK_LEN;
      curChar = in[0];
      in++;
      if ((curChar >> 6) == 0x2) {
        CHECK_LEN;
        curChar = in[0];
        in++;
        if ((curChar >> 6) == 0x2) return 3;
      }
    } else if ((curChar >> 3) == 0x1e) {
      /* four byte */
      CHECK_LEN;
      curChar = in[0];
      in++;
      if ((curChar >> 6) == 0x2) {
        CHECK_LEN;
        curChar = in[0];
        in++;
        if ((curChar >> 6) == 0x2) {
          CHECK_LEN;
          curChar = in[0];
          in++;
          if ((curChar >> 6) == 0x2) return 4;
        }
      }
    }
  }

  // error case
  return -1;
}

/*
 * Scans the current position of the buffer
 * returning the total number of UTF8 characters found
 */
size_t utf8CharCount(unsigned char *in, size_t in_len) {
  size_t total = 0, leftOver = in_len;
  int8_t len = 0;
  unsigned char *start = in;

  if (in_len > 0) {
    while (leftOver) {
      len = utf8CharLen(start, leftOver);
      leftOver -= len;
      start += len;
      total++;
    }
  }

  return total;
}
