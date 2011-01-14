#include "ext.h"

#include "string_utf8.h"
#include "string_scanner_utf8.h"

ID intern_as_utf8;

#ifdef HAVE_RUBY_ENCODING_H
rb_encoding *utf8Encoding;
#endif

void Init_utf8() {
  init_String_UTF8();
  init_StringScanner_UTF8();

  intern_as_utf8 = rb_intern("as_utf8");

#ifdef HAVE_RUBY_ENCODING_H
  utf8Encoding = rb_utf8_encoding();
#endif
}
