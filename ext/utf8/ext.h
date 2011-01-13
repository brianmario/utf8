#ifndef UTF8_EXT_H
#define UTF8_EXT_H

#include <ruby.h>

#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
extern rb_encoding *utf8Encoding;
#define AS_UTF8(_str)                         \
  _str = rb_funcall(_str, intern_as_utf8, 0); \
  rb_enc_associate(_str, utf8Encoding);

#else
#define AS_UTF8(_str) _str = rb_funcall(_str, intern_as_utf8, 0)
#endif

#endif