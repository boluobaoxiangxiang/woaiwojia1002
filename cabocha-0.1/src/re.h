/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: re.h,v 1.6 2001/06/28 07:30:19 taku-ku Exp $;

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

#ifndef _CABOCHA_RE_H
#define _CABOCHA_RE_H
#include "cabocha.h"
#include "common.h"
//extern "C" {
//#include "regex.h"
//}

namespace CaboCha
{
class Regex
{
 private:
  struct re_pattern_buffer *rp;
  struct re_registers *rg;
  string *str;
  
 public: 
  Regex(int type = DEFAULT_MBCTYPE);
  ~Regex();
  int compile(const string &);
  int search(string &);
  const string getItem(unsigned int);

  int operator==(string& s) { return (-1 != search(s) ? 1 : 0); };
  int operator!=(string& s) { return (-1 != search(s) ? 0 : 1); };
};
}

#endif


