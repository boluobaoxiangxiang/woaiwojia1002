#include "ruby.h"
#include "cabocha.h"

/////////////////////////////////////////////////
//
//  Ruby interface for CaboCha
//
//  $Id: CaboCha.cc,v 1.1 2001/05/03 03:42:39 taku-ku Exp $;

#ifdef __cplusplus

namespace CaboCha {
		
extern "C" {
  static VALUE ParserClass;
  static VALUE CaboCha_new(int, VALUE *, VALUE);
  static void  CaboCha_destroy(CaboCha *);
  static VALUE CaboCha_parse(VALUE, VALUE);
  void Init_CaboCha(void);
}

static VALUE
CaboCha_new(int argc, VALUE *argv, VALUE self)
{
  Param p;
  VALUE fn;
  rb_scan_args(argc, argv, "01", &fn);
  if (fn != Qnil) {
    Check_SafeStr(fn);
    p.set(string(STR2CSTR(fn)));
  } else {
    p.set(" "); // dummy
  }

  CaboCha *parser = new CaboCha(p);
  return Data_Wrap_Struct(self, 0, (void(*)())CaboCha_destroy, parser);
}

static void
CaboCha_destroy(CaboCha *parser)
{
  delete parser;
}

static VALUE
CaboCha_parse(VALUE self, VALUE str)
{
  Check_SafeStr(str);
  CaboCha *parser;
  Data_Get_Struct(self, CaboCha, parser);
  char *p = (char *)parser->sparse_tostr(STR2CSTR(str));
  return rb_str_new2(p);
}

void
Init_CaboCha(void)
{
  ParserClass = rb_define_class("CaboCha", rb_cObject);
  rb_define_singleton_method(ParserClass, "new", (VALUE(*)())CaboCha_new, -1);
  rb_define_method(ParserClass, "parse",         (VALUE(*)())CaboCha_parse,  1);
}
}
#endif
