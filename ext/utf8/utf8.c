#include <stdio.h>
#include <stdint.h>

#define CHECK_LEN if ((size_t)(in-start) >= in_len) return -1;

/*
 * Scans the current position of the buffer
 * returning the length of this UTF-8 character
 */
int8_t utf8CharLen(unsigned char *in, size_t in_len) {
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
    } else if ((curChar >> 2) == 0x3e) {
      /* five byte */
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
          if ((curChar >> 6) == 0x2) {
            CHECK_LEN;
            curChar = in[0];
            in++;
            if ((curChar >> 6) == 0x2) return 5;
          }
        }
      }
    } else if ((curChar >> 2) == 0x7e) {
      /* six byte */
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
          if ((curChar >> 6) == 0x2) {
            CHECK_LEN;
            curChar = in[0];
            in++;
            if ((curChar >> 6) == 0x2) {
              CHECK_LEN;
              curChar = in[0];
              in++;
              if ((curChar >> 6) == 0x2) return 6;
            }
          }
        }
      }
    }
  }

  // error case
  return -1;
}

/*
 * Scans the current position of the buffer
 * returning the total number of UTF-8 characters found
 */
int64_t utf8CharCount(unsigned char *in, size_t in_len) {
  int64_t total = 0, leftOver = in_len;
  int8_t len = 0;
  unsigned char *start = in;

  if (in_len > 0) {
    while (leftOver) {
      len = utf8CharLen(start, leftOver);
      if (len < 0) {
        return -1;
      }
      leftOver -= len;
      start += len;
      total++;
    }
  }

  return total;
}

/*
 * Scans the current position of the buffer
 * returning the codepoint for the UTF-8 character at this position
 */
int32_t utf8CharToCodepoint(unsigned char *in, size_t in_len) {
  int32_t cp, ncp, len;

  len = utf8CharLen(in, in_len);
  cp = *in++;
  if (len > 1) {
    len--;
    ncp = cp & ((1 << (6 - len)) - 1);
    while (len--) {
      cp = *in++;
      ncp = (ncp << 6) | (cp & ((1 << 6) - 1));
    }
    return ncp;
  } else {
    return cp;
  }
}
