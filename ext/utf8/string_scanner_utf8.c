#include "ext.h"
#include "ruby/regex.h"
#include "utf8.h"

extern ID intern_as_utf8;

struct strscanner {
    /* multi-purpose flags */
    unsigned long flags;

    /* the string to scan */
    VALUE str;

    /* scan pointers */
    long prev; /* legal only when MATCHED_P(s) */
    long curr; /* always legal */

    /* the regexp register; legal only when MATCHED_P(s) */
    struct re_registers regs;
};

#define GET_SCANNER(obj, var)                                                          \
    Data_Get_Struct(obj, struct strscanner, var);                                      \
    if (NIL_P(var->str)) rb_raise(rb_eArgError, "uninitialized StringScanner object");

static VALUE rb_cStringScanner_UTF8_getch(VALUE self) {
  unsigned char *str;
  size_t len;
  struct strscanner *scanner;
  VALUE rb_raw_offset, utf8Str;
  size_t raw_offset = 0;
  int8_t lastCharLen=0;
  GET_SCANNER(self, scanner);

  rb_raw_offset = rb_iv_get(self, "raw_offset");
  if (NIL_P(rb_raw_offset)) {
    rb_iv_set(self, "raw_offset", INT2FIX(0));
  } else {
    raw_offset = FIX2ULONG(rb_raw_offset);
  }

  str = (unsigned char *)RSTRING_PTR(scanner->str);
  len = RSTRING_LEN(scanner->str);

  if (len > 0 && len > raw_offset) {
    lastCharLen = utf8CharLen(str, len);
    utf8Str = rb_str_new((char *)str+raw_offset, lastCharLen);
    raw_offset += lastCharLen;
    rb_iv_set(self, "raw_offset", LONG2FIX(raw_offset));
    AS_UTF8(utf8Str);
    return utf8Str;
  } else {
    return Qnil;
  }
}

static VALUE rb_cStringScanner_UTF8_reset(int argc, VALUE * argv, VALUE self) {
  VALUE retVal;

  retVal = rb_call_super(argc, argv);
  rb_iv_set(self, "raw_offset", Qnil);

  return retVal;
}

void init_StringScanner_UTF8() {
  ID intern_string_scanner = rb_intern("StringScanner");
  VALUE rb_cStringScanner, rb_cStringScanner_UTF8;

  if (!rb_const_defined(rb_cObject, intern_string_scanner)) {
    rb_require("strscan");
  }
  rb_cStringScanner = rb_const_get(rb_cObject, intern_string_scanner);
  rb_cStringScanner_UTF8 = rb_define_class_under(rb_cStringScanner, "UTF8", rb_cStringScanner);

  rb_define_method(rb_cStringScanner_UTF8, "getch", rb_cStringScanner_UTF8_getch, 0);
  rb_define_method(rb_cStringScanner_UTF8, "reset", rb_cStringScanner_UTF8_reset, -1);
}
