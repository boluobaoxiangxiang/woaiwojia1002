/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: re.cc,v 1.9 2001/06/28 07:30:19 taku-ku Exp $;

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

#include "re.h"
#define MEMZERO(p,type,n) memset((p), 0, sizeof(type)*(n))
#define xfree(x)        { if ((x)) free((x)), (x) = 0; }

namespace CaboCha
{
//Regex::Regex(int type = DEFAULT_MBCTYPE)
Regex::Regex(int type)
{
  re_mbcinit(type);
  rp = 0;
  rg = 0;
}

Regex::~Regex()
{
  if (rp) { re_free_pattern(rp); };
  if (rg) { re_free_registers(rg); xfree(rg); };
}

int Regex::compile(const string &pat)
{
  if (rp) { re_free_pattern(rp); };
  if (rg) { re_free_registers(rg); xfree(rg); };
  rp = (struct re_pattern_buffer *)malloc(sizeof(struct re_pattern_buffer));
  MEMZERO((char *)rp, struct re_pattern_buffer, 1);
  rp->allocated = 0;
  rp->buffer  = 0;
  rp->fastmap = 0;

  rg = (struct re_registers *)malloc(sizeof(struct re_registers));
  MEMZERO((char *)rp, struct re_registers, 1);
  rg->allocated = 0;
  re_compile_pattern(pat.c_str(), pat.size(), rp);
  return 1;
}

int Regex::search(string& s)
{
  if (! rp || ! rg) return -1;
  str = &s;
  return re_search(rp, s.c_str(), s.size(), 0, s.size(), rg);
}

const string Regex::getItem(unsigned int i)
{
  return str->substr(rg->beg[i], (rg->end[i] - rg->beg[i]));
}
}
