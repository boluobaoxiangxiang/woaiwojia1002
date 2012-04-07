/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: chasenio.cc,v 1.5 2001/06/28 07:30:19 taku-ku Exp $;

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

#include "chasenio.h"

#ifndef _WIN32
extern "C" {
#include <stdio.h>
}
#endif

#define CHASEN_DEFAULT_FORMAT "%m\\t%y\\t%M\\t%U(%P-)\\t%T \\t%F \\n"

#ifdef _WIN32
#define CHASEN_DLNAME           "chasen.dll"
#define CHASEN_INIT_PROC_NAME   "set_argument_parameters"
#define CHASEN_PARSE_PROC_NAME  "analyze"
#else
#define CHASEN_DLNAME           "libchasen.so"
#define CHASEN_INIT_PROC_NAME   "chasen_getopt_argv"
#define CHASEN_PARSE_PROC_NAME  "chasen_sparse_tostr"
#endif

namespace CaboCha
{
ChaSen::ChaSen(Param &p)
{
  int size = 0;
  char *option[5] = {0,0,0,0,0};
  option[0] = "CaboCha::ChaSen"; 
  if (p.chasenrc != "") {
    option[1] = "-r";
    option[2] = const_cast<char *>(p.chasenrc.c_str());
    option[3] = "-F";
    option[4] = CHASEN_DEFAULT_FORMAT;
    size = 5;
  } else {
    option[1] = "-F";
    option[2] = CHASEN_DEFAULT_FORMAT;
    size = 3;
  }

  if (dl.load (CHASEN_DLNAME)) {
    fpInit  = dl.getProcAddr (CHASEN_INIT_PROC_NAME);
    fpParse = dl.getProcAddr (CHASEN_PARSE_PROC_NAME);
  }

  if ( ! dl.isloaded() || ! fpInit || ! fpParse) {
    cerr << "Fatal:  Load Error " <<  CHASEN_DLNAME << endl;
    exit(EXIT_FAILURE);
  }

#ifdef _WIN32
  (*fpInit)(size,   option);
#else
  (*fpInit)(option, stderr);
#endif
}

int ChaSen::parse() 
{ 
  out = (char *)(*fpParse)(const_cast<char *>(line.c_str()));
  return 1; 
};

istream &ChaSen::read(istream &is) 
{ 
   if (! getline(is, line)) is.clear(ios::eofbit|ios::badbit);
  return is; 
}

ostream &ChaSen::write(ostream &os) 
{ 
  os << out; 
  return os; 
}
}
