/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: selector.h,v 1.7 2001/06/18 06:03:26 taku-ku Exp $;

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

#ifndef _CABOCHA_SELECTOR_H_
#define _CABOCHA_SELECTOR_H_

#include "cabocha.h"
#include "re.h"

namespace CaboCha
{
static const string KUTOUTEN_PAT      =  "(¡£|¡¢)";
static const string OPEN_BRACKET_PAT  =  "(¡Ê|¡Æ|¡È|¡Ô|¡Ö|¡Ø|¡Î|¡Ò|¡Ð)";
static const string CLOSE_BRACKET_PAT =  "(¡Ë|¡Ç|¡É|¡Õ|¡×|¡Ù|¡Ï|¡Ó|¡Ñ)";
static const string DYN_A_PAT         =  "^(½õ»ì|Éû»ì|Ï¢ÂÎ»ì|ÀÜÂ³»ì)";
static const string CASE_PAT          =  "^(½õ»ì)";
static const string IPA_FUNC_PAT      =  "^(½õ»ì|½õÆ°»ì|Æ°»ì\\-Èó¼«Î©|Æ°»ì\\-ÀÜÈø|·ÁÍÆ»ì\\-Èó¼«Î©|·ÁÍÆ»ì\\-ÀÜÈø)";
static const string IPA_HEAD_PAT      =  "^(µ­¹æ)";
static const string JUMAN_FUNC_PAT    =  "^(ÆÃ¼ì)";
static const string JUMAN_HEAD_PAT    =  "^(ÆÃ¼ì|½õ»ì|ÀÜÈø¼­)";

class Selector: public Base
{
  int mode;
  Regex re_kutouten, re_open_bracket, re_close_bracket;
  Regex re_dyn_a, re_case;
  Regex re_ipa_func, re_ipa_head;
  Regex re_juman_func, re_juman_head;

public:
  vector < vector < vector <string> > > morph;
  vector < vector <string> > morph2;
  vector <string> depinfo;
  enum {NONE, IPA, JUMAN};

  Selector(Param &);
  ~Selector();
  int parse();
  istream &read(istream &);
  ostream &write(ostream &);

  unsigned int size() { return morph.size(); }

  int clear()
  {
    morph.clear();
    morph2.clear();
    depinfo.clear();
    return 1;
  }
};

}
#endif
