/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: chunker.h,v 1.4 2001/06/18 06:03:26 taku-ku Exp $;

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

#ifndef _CABOCHA_CHUNKER_H_
#define _CABOCHA_CHUNKER_H_

#include "cabocha.h"

namespace CaboCha
{
class Chunker: public Base
{
private:
  YamCha::Parser chunker;
  vector < unsigned int >    result;
  vector < string > lineBuf;

public:
  Chunker(Param &);
  ~Chunker();
  int parse();
  istream &read(istream &);
  ostream &write(ostream &);

  unsigned int size()
  {
    return result.size();
  }

  int clear()
  {
    lineBuf.clear();
    result.clear();
    return 1;
  }
};

}
#endif
