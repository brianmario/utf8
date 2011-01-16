#include "ext.h"
#include "utf8.h"

extern ID intern_as_utf8;

#ifndef RUBINIUS
struct strscanner {
    /* multi-purpose flags */
    unsigned long flags;

    /* the string to scan */
    VALUE str;

    /* scan pointers */
    long prev; /* legal only when MATCHED_P(s) */
    long curr; /* always legal */

    /*
     * We never access this member, and would require a shitload of other patching
     * to work right on other ruby versions
     *
     */
    /* the regexp register; legal only when MATCHED_P(s) */
    /* struct re_registers regs; */
};

#define GET_SCANNER(obj, var)                                                          \
    Data_Get_Struct(obj, struct strscanner, var);                                      \
    if (NIL_P(var->str)) rb_raise(rb_eArgError, "uninitialized StringScanner object");
#endif

/*
 * Document-class: StringScanner::UTF8
 */

/*
 * call-seq: getch
 *
 * Works like StringScanner#getch but is UTF8-aware
 */
static VALUE rb_cStringScanner_UTF8_getch(VALUE self) {
  unsigned char *str;
  long len = 0, pos = 0;
  VALUE utf8Str, curStr;
  int8_t lastCharLen=0;
  
#ifndef RUBINIUS
  struct strscanner *scanner;
  GET_SCANNER(self, scanner);

  curStr = scanner->str;
  pos = scanner->curr;
#else
  curStr = rb_iv_get(self, "@string");
  pos = FIX2LONG(rb_iv_get(self, "@pos"));
#endif

  str = (unsigned char *)RSTRING_PTR(curStr);
  len = RSTRING_LEN(curStr);

  if (len > 0 && len > pos) {
    lastCharLen = utf8CharLen(str, len);
    if (lastCharLen < 0) {
      rb_raise(rb_eArgError, "invalid utf-8 byte sequence");
    }
    utf8Str = rb_str_new((char *)str+pos, lastCharLen);
    pos += lastCharLen;
#ifndef RUBINIUS
    scanner->curr = pos;
#else
    rb_iv_set(self, "@pos", LONG2FIX(pos));
#endif
    AS_UTF8(utf8Str);
    return utf8Str;
  } else {
    return Qnil;
  }
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
}
