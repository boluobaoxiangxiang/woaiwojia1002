/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: cabocha.cc,v 1.11 2001/06/18 06:03:26 taku-ku Exp $;

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

#include "cabocha.h"
#include "common.h"

static inline void do_parse(CaboCha::Base &p, istream &is, ostream &os)
{
  while (is >> p) {
    p.parse();
    os << p;
  }
}

int main (int argc, char **argv)
{
  CaboCha::Param param;
  param.set(argc, argv);

  CaboCha::CaboCha parser(param);
  ostream *ofs = &cout;

  if (param.outputFileName != "") {
    ofs = new ofstream(param.outputFileName.c_str());
    if (! *ofs) {
      cerr << argv[0] << ": " << param.outputFileName
           << ": permission denied" << endl;
      exit(EXIT_FAILURE);
    }
  }

  if (param.inputFileNameList.size()) {
    for (unsigned int i = 0; i < param.inputFileNameList.size(); i++) {
      ifstream ifs(param.inputFileNameList[i].c_str());
      if (! ifs) {
        cerr << argv[0] << ": " << param.inputFileNameList[i]
             << ": no such file or directory" << endl;
        continue;
      }
      do_parse(parser, ifs, *ofs);
    }
  } else {
    do_parse(parser, cin, *ofs);
  }

  if (ofs != &cout) delete ofs;
  exit(EXIT_SUCCESS);
}

