#include <stdio.h>
#include <stdint.h>

/* C shit */
#define CHECK_LEN if ((size_t)(in-start) >= in_len) return 0;

#define AS_UTF8(str) rb_funcall(str, intern_as_utf8, 0)

static inline int8_t charLen(unsigned char *in, size_t in_len) {
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

static size_t totalCharCount(unsigned char *in, size_t in_len) {
  size_t total = 0, leftOver = in_len;
  int8_t len = 0;
  unsigned char *start = in;

  if (in_len > 0) {
    while (leftOver) {
      len = charLen(start, leftOver);
      leftOver -= len;
      start += len;
      total++;
    }
  }

  return total;
}

/* Ruby shit */
#include <ruby.h>
static VALUE intern_as_utf8;

static VALUE rb_cString_UTF8_length(VALUE self) {
  unsigned char *str = (unsigned char *)RSTRING_PTR(self);
  size_t len = RSTRING_LEN(self);
  size_t utf8_len = 0;

  utf8_len = totalCharCount(str, len);

  return INT2FIX(utf8_len);
}

static VALUE rb_cString_UTF8_each_char(VALUE self) {
  unsigned char *str = (unsigned char *)RSTRING_PTR(self);
  size_t len = RSTRING_LEN(self), i=0;
  int8_t lastCharLen=0;

  // this will return an Enumerator wrapping this string, yielding this method
  // when Enumerator#each is called
  RETURN_ENUMERATOR(self, 0, 0);

  for(; i<len; i+=lastCharLen) {
    lastCharLen = charLen(str, len);
    rb_yield(AS_UTF8(rb_str_new((char *)str+i, lastCharLen)));
  }

  return self;
}

static VALUE rb_cString_UTF8_slice(int argc, VALUE *argv, VALUE self) {
  unsigned char *str = (unsigned char *)RSTRING_PTR(self), *start = str;
  size_t len = RSTRING_LEN(self);

  if (len == 0) return Qnil;

  if (argc == 2) {
    if (TYPE(argv[0]) == T_REGEXP) {
      rb_raise(rb_eArgError, "Regular Expressions aren't supported yet");
    }

    // [offset, length] syntax
    long wantPos = NUM2LONG(argv[0]), curPos = 0, wantLen = NUM2LONG(argv[1]);
    int8_t curCharLen = 0;
    unsigned char *offset = str;

    if (wantLen < 0) {
      return Qnil;
    }

    if (wantPos < 0) {
      long char_cnt = totalCharCount(str, len);
      if ((wantPos * -1) > char_cnt) {
        return Qnil;
      }
      wantPos = char_cnt + wantPos;
    }

    // scan until starting position
    curCharLen = charLen(str, len);
    while (curPos < wantPos) {
      // if we're about to step out of bounds, return nil
      if ((size_t)(str-start) >= len) {
        return Qnil;
      }

      str += curCharLen;
      curCharLen = charLen(str, len);
      curPos++;
    }

    // now scan until we have the number of chars asked for
    curPos = 1;
    offset = str;
    str += curCharLen;
    curCharLen = charLen(str, len);
    while (curPos < wantLen) {
      // if we're about to step out of bounds, stop
      if ((size_t)(str-start) >= len) {
        break;
      }

      str += curCharLen;
      curCharLen = charLen(str, len);
      curPos++;
    }

    return AS_UTF8(rb_str_new((char *)offset, str-offset));
  }

  if (argc != 1) {
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 1)", argc);
  }

  // [Fixnum] syntax
  if (TYPE(argv[0]) == T_FIXNUM) {
    long wantPos = NUM2LONG(argv[0]), curPos = 0;
    int8_t curCharLen = 0;

    if (wantPos < 0) {
      long char_cnt = totalCharCount(str, len);
      if ((wantPos * -1) > char_cnt) {
        return Qnil;
      }
      wantPos = char_cnt + wantPos;
    }

    curCharLen = charLen(str, len);
    while (curPos < wantPos) {
      // if we're about to step out of bounds, return nil
      if ((size_t)(str-start) >= len) {
        return Qnil;
      }

      str += curCharLen;
      curCharLen = charLen(str, len);
      curPos++;
    }

    return AS_UTF8(rb_str_new((char *)str, curCharLen));
  } else {
    // [Range] syntax
    long wantPos, curPos = 0, wantLen, char_cnt = 0;
    int8_t curCharLen = 0;
    unsigned char *offset = str;
    VALUE ret;

    char_cnt = totalCharCount(str, len);
    ret = rb_range_beg_len(argv[0], &wantPos, &wantLen, char_cnt, 0);

    if (ret == Qnil) {
      return Qnil;
    } else if (ret == Qfalse) {
      // TODO: wtf do we do :P
    }

    if (wantLen == 0) {
      return AS_UTF8(rb_str_new("", 0));
    }

    // scan until starting position
    curCharLen = charLen(str, len);
    while (curPos < wantPos) {
      // if we're about to step out of bounds, return ""
      if ((size_t)(str-start) >= len) {
        return AS_UTF8(rb_str_new("", 0));
      }

      str += curCharLen;
      curCharLen = charLen(str, len);
      curPos++;
    }

    // now scan until we have the number of chars asked for
    curPos = 1;
    offset = str;
    str += curCharLen;
    curCharLen = charLen(str, len);
    while (curPos < wantLen) {
      // if we're about to step out of bounds, stop
      if ((size_t)(str-start) >= len) {
        break;
      }

      str += curCharLen;
      curCharLen = charLen(str, len);
      curPos++;
    }

    return AS_UTF8(rb_str_new((char *)offset, str-offset));
  }
}

void Init_utf8() {
  VALUE rb_cString_UTF8 = rb_define_class_under(rb_cString, "UTF8", rb_cString);

  rb_define_method(rb_cString_UTF8, "length",    rb_cString_UTF8_length, 0);
  rb_define_method(rb_cString_UTF8, "size",      rb_cString_UTF8_length, 0);

  rb_define_method(rb_cString_UTF8, "each_char", rb_cString_UTF8_each_char, 0);
  rb_define_method(rb_cString_UTF8, "chars",     rb_cString_UTF8_each_char, 0);

  rb_define_method(rb_cString_UTF8, "[]",        rb_cString_UTF8_slice, -1);
  rb_define_method(rb_cString_UTF8, "slice",     rb_cString_UTF8_slice, -1);

  intern_as_utf8 = rb_intern("as_utf8");
}