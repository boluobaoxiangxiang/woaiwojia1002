/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: libcabocha.cc,v 1.15 2001/07/07 21:02:40 taku-ku Exp $;

 Copyright (C) 2001 Taku Kudoh  All rights reserved.
 This is free software with ABSOLUTELY NO WARRANTY.

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA
*/

#include <cabocha.h>
#include "common.h"
#include "chunker.h"
#include "parser.h"
#include "selector.h"
#include "chasenio.h"

cabocha_t *cabocha_getopt_argv(int argc, char **argv)
{
  cabocha_t *c = new cabocha_t;
  if (!c ) return 0;
  c->allocated = 1;
  CaboCha::Param p;
  p.set(argc,argv);
  c->ptr = (void *)(new CaboCha::CaboCha(p));
  if (! c->ptr) return 0;
  return c;
}

cabocha_t *cabocha_getopt_argv2 (char *arg)
{
  cabocha_t *c = new cabocha_t;
  if (!c ) return 0;
  c->allocated = 1;
  CaboCha::Param p;
  p.set(string(arg));
  c->ptr = (void *)(new CaboCha::CaboCha(p));
  if (! c->ptr) return 0;
  return c;
}

char *cabocha_sparse_tostr(cabocha_t *c, char *s)
{
  if (c && c->allocated) {
    CaboCha::CaboCha *p = (CaboCha::CaboCha *)(c->ptr);
    return const_cast<char *>(p->sparse_tostr(s));
  } else
    return 0;
}

void cabocha_destroy(cabocha_t *c)
{
  if (c && c->allocated) delete c;
  c = 0;
}

namespace CaboCha 
{
  template <class T>
  static Base * newBase(Param &p)
  {
    return (Base *)(new T (p));
  }
   
  CaboCha::CaboCha(Param &q)
  {
    int inputLayer  = q.inputLayer;
    int outputLayer = q.outputLayer;

    switch (inputLayer) {
    case Param::INPUT_RAW_SENTENCE:  // case 1
      {
	switch (outputLayer) {
	case Param::OUTPUT_POS: 
	  append(newBase<ChaSen>(q));
	  break;
	case Param::OUTPUT_CHUNK:
	  append(newBase<ChaSen>(q));
	  append(newBase<Chunker>(q));
	  break;
	case Param::OUTPUT_SELECTION:
	  append(newBase<ChaSen>(q));
	  append(newBase<Chunker>(q));
	  append(newBase<Selector>(q));
	  break;
	case Param::OUTPUT_PARSE:
	  append(newBase<ChaSen>(q));
	  append(newBase<Chunker>(q));
	  append(newBase<Selector>(q));
	  append(newBase<Parser>(q));
	  break;
	}
	break;
      }

    case Param::INPUT_POS: // case 2
      {
	switch (outputLayer) {
	case Param::OUTPUT_POS:
	  break;
	case Param::OUTPUT_CHUNK:
	  append(newBase<Chunker>(q));
	  break;
	case Param::OUTPUT_SELECTION:
	  append(newBase<Chunker>(q));
	  append(newBase<Selector>(q));
	  break;
	case Param::OUTPUT_PARSE:
	  append(newBase<Chunker>(q));
	  append(newBase<Selector>(q));
	  append(newBase<Parser>(q));
	  break;
	}
	break;
      }

    case Param::INPUT_CHUNK: // case 3
      {
	switch (outputLayer) {
	case Param::OUTPUT_POS:
	case Param::OUTPUT_CHUNK: 
	  break;
	case Param::OUTPUT_SELECTION:
	  append(newBase<Selector>(q));
	  break;
	case Param::OUTPUT_PARSE:
	  append(newBase<Selector>(q));
	  append(newBase<Parser>(q));
	  break;
	}
	break;
      }
      
    case  Param::INPUT_SELECTION: // case 4
      {
	switch (outputLayer) {
	case Param::OUTPUT_POS:
	case Param::OUTPUT_CHUNK:
	case Param::OUTPUT_SELECTION:
	  break;
	case Param::OUTPUT_PARSE:
	  append(newBase<Parser>(q));
	  break;
	}
	break;
      }
    }

    if (list.size() == 0) {
      cerr << "Invalid layer type, Input=" << 
	 inputLayer << " Output=" << outputLayer << endl << endl;
      CABOCHA_EXIT;
    }
  }

  CaboCha::~CaboCha()
  {
    for (unsigned int i = 0; i < list.size(); i++) delete list[i];
  }

  ostream &CaboCha::write(ostream &os)
  {
    return list[list.size()-1]->write(os);
  }

  istream &CaboCha::read(istream &is)
  {
    return list[0]->read(is);
  }

  int CaboCha::parse() 
  {
    // it's very dirty code
    switch (list.size()) {
    case 1:
      list[0]->parse();
      break;
    case 2:
      (*list[0] | *list[1]);
      break;
    case 3:
      (*list[0] | *list[1] | *list[2]);
      break;
    case 4:
      (*list[0] | *list[1] | *list[2] | *list[3]);
      break;
    case 5:
      (*list[0] | *list[1] | *list[2] | *list[3] | *list[4]);
      break;
    case 6:
      (*list[0] | *list[1] | *list[2] | *list[3] | *list[4] | *list[5]);
      break;
    case 7:
      (*list[0] | *list[1] | *list[2] | *list[3] | *list[4] | *list[5]);
      break;
    case 8:
      (*list[0] | *list[1] | *list[2] | *list[3] | *list[4] | *list[5] | *list[6] | *list[7]);
      break;
    }

    return 1;
  }

  const char *CaboCha::sparse_tostr(const char* str) 
  {
    if (!str) return 0;
    strstream istrs;
    istrs << str << '\0' << flush;
    this->read(istrs);
    this->parse();
    strstream ostrs;
    this->write(ostrs);
    ostrs << '\0' << flush;
    outbuf = ostrs.str();
    return outbuf.c_str();
  }
}

