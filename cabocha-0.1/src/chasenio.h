/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: chasenio.h,v 1.9 2001/06/19 09:30:23 taku-ku Exp $;

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

#ifndef _CABOCHA_CHASENIO_H
#define _CABOCHA_CHASENIO_H

#include "common.h"
#include "cabocha.h"
#include "DynamicLibrary.h"

namespace CaboCha
{
class ChaSen: public Base
{
private:
  string line;
  char *out;
  DynamicLibrary dl;
  DLPROC fpInit;
  DLPROC fpParse;

public:
  ChaSen(Param &);
  ~ChaSen() {};
  int parse();
  istream &read(istream &is);
  ostream &write(ostream &os);

  unsigned int size() { return 1; };
  int clear() { return 1; };
};
}

#endif
