#include "cabocha.h"
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#define CaboCha__CaboCha CaboCha::CaboCha

/* $Id: CaboCha.xs,v 1.2 2001/06/18 16:05:24 taku-ku Exp $ */

MODULE = Text::CaboCha PACKAGE = Text::CaboCha

CaboCha::CaboCha *
CaboCha::CaboCha::new(s)
        char *s;
	PREINIT:
	CaboCha::Param p;
        CODE:
	p.set(string(s) + " ");
	CaboCha::CaboCha *c = new CaboCha::CaboCha(p);
        RETVAL = c;
        OUTPUT:
        RETVAL

char *
CaboCha::CaboCha::parse(s)
	char *s
	CODE:
	RETVAL = (char *)THIS->sparse_tostr(s);
	OUTPUT:
	RETVAL

void
CaboCha::CaboCha::DESTROY()
