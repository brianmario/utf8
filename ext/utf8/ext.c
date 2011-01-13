#include "ext.h"

#include "string.h"
// #include "string_scanner.h"

void Init_utf8() {
  init_String_UTF8();
  // init_StringScanner_UTF8();

#ifdef HAVE_RUBY_ENCODING_H
  utf8Encoding = rb_utf8_encoding();
#endif
}
