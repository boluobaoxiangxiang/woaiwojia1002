/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: selector.cc,v 1.12 2001/06/30 20:14:05 taku-ku Exp $;

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

#ifdef _WIN32
#include "selector-sjis.h"
#else
#include "selector.h"
#endif

#include "common.h"
#include "regex.h"

namespace CaboCha {

Selector::Selector(Param &) 
{
  mode = NONE;
  re_kutouten.compile(KUTOUTEN_PAT);
  re_open_bracket.compile(OPEN_BRACKET_PAT);
  re_close_bracket.compile(CLOSE_BRACKET_PAT);
  re_dyn_a.compile(DYN_A_PAT);
  re_case.compile(CASE_PAT);
  re_ipa_func.compile(IPA_FUNC_PAT);
  re_ipa_head.compile(IPA_HEAD_PAT);
  re_juman_func.compile(JUMAN_FUNC_PAT);
  re_juman_head.compile(JUMAN_HEAD_PAT);
}

Selector::~Selector() {}

istream &Selector::read(istream &is)
{
  clear();
  int oldmode = NONE;
  
  while (1) {
    string line;
    if (! getline(is,line)) {
//      is.setstate(ios::eofbit);
      is.clear(ios::eofbit|ios::badbit);
      return is;
    }

    if (line == "EOS" || line == "") return is;

    vector <string> tmp;
    YamCha::split_string(line,"\t ",tmp);

    if (tmp[0] == "*" && tmp.size() == 3) {
      vector <string> tmpv; 
      morph2.push_back(tmpv);
      vector < vector <string> > tmpv2; 
      morph.push_back(tmpv2);
      depinfo.push_back(tmp[2]);
    } else {
      if (tmp.size() == 7) mode = JUMAN;
      else if (tmp.size() > 3 && tmp.size() < 7) mode = IPA;
      if ((oldmode != NONE && oldmode != mode) || morph.size() == 0) {
        cerr << "CaboCha::Selector:  Format error [" << line << "]" << endl;
	exit(EXIT_FAILURE);
      }
      oldmode = mode;
      morph[morph.size()-1].push_back(tmp);
      morph2[morph2.size()-1].push_back(line);
    }
  }
}

ostream &Selector::write(ostream &os)
{
  for (unsigned int i = 0; i < morph.size(); i++) {
    unsigned int func_id = 0;
    unsigned int head_id = 0;
    os << "* " << i << " " << depinfo[i] << " ";

    if (mode == JUMAN) {
      for (unsigned int j = 0; j < morph[i].size(); j++) { // detect head and func
	if (-1 == re_juman_func.search(morph[i][j][3])) func_id = j;
	if (-1 == re_juman_head.search(morph[i][j][3])) head_id = j;
      }

      os << head_id << "/" << func_id;
      
      if (morph[i][head_id][0] != "*") os << " F_H0:" << morph[i][head_id][0];
      if (morph[i][head_id][3] != "*") os << " F_H1:" << morph[i][head_id][3];
      if (morph[i][head_id][4] != "*") os << " F_H2:" << morph[i][head_id][4];
      if (morph[i][head_id][5] != "*") os << " F_H3:" << morph[i][head_id][5];
      if (morph[i][head_id][6] != "*") os << " F_H4:" << morph[i][head_id][6];

      if (morph[i][func_id][0] != "*") os << " F_F0:" << morph[i][func_id][0];
      if (morph[i][func_id][3] != "*") os << " F_F1:" << morph[i][func_id][3];
      if (morph[i][func_id][4] != "*") os << " F_F2:" << morph[i][func_id][4];
      if (morph[i][func_id][5] != "*") os << " F_F3:" << morph[i][func_id][5];
      if (morph[i][func_id][6] != "*") os << " F_F4:" << morph[i][func_id][6];

      if (-1 != re_case.search(morph[i][func_id][3])) os << " G_CASE:" << morph[i][func_id][0];
      
      if (morph[i][head_id][4] != "*") 
	os << " B:" << morph[i][head_id][3] << "/" << morph[i][head_id][4];
      else 
	os << " B:" << morph[i][head_id][3];

      if (-1 != re_dyn_a.search(morph[i][func_id][3])) 
	os << " A:" << morph[i][func_id][0];
      else if (morph[i][func_id][6] != "*") 
	os << " A:" << morph[i][func_id][6];
      else if (morph[i][func_id][3] != "*" && morph[i][func_id][4] != "*") 
	os << " A:" << morph[i][func_id][3] << "/" << morph[i][func_id][4];
      else 
	os << " A:" << morph[i][func_id][3];

    } else if (mode == IPA) {
      for (unsigned int j = 0; j < morph[i].size(); j++) { // detect head and func
	if (-1 != re_ipa_func.search(morph[i][j][3]))      func_id = j;
	else if (-1 == re_ipa_head.search(morph[i][j][3])) head_id = j;
      }
      if (head_id > func_id) func_id = head_id;
      os << head_id << "/" << func_id;
      
      vector <string> head_pos_list;
      vector <string> func_pos_list; 
      YamCha::split_string(morph[i][head_id][3], "-", head_pos_list);
      YamCha::split_string(morph[i][func_id][3], "-", func_pos_list);

      if (morph[i][head_id][0] != "*") os << " F_H0:" << morph[i][head_id][0];
      if (head_pos_list.size() > 0)    os << " F_H1:" << head_pos_list[0];
      if (head_pos_list.size() > 1)    os << " F_H2:" << head_pos_list[1];
      if (head_pos_list.size() > 2)    os << " F_H3:" << head_pos_list[2];
      if (head_pos_list.size() > 3)    os << " F_H4:" << head_pos_list[3];
      if (morph[i][head_id].size() > 4 && morph[i][head_id][4] != "*" && morph[i][head_id][4] != "") 
	os << " F_H5:" << morph[i][head_id][4];
      if (morph[i][head_id].size() > 5 && morph[i][head_id][5] != "*" && morph[i][head_id][5] != "") 
	os << " F_H6:" << morph[i][head_id][5];

      if (morph[i][func_id][0] != "*") os << " F_F0:" << morph[i][func_id][0];
      if (func_pos_list.size() > 0)    os << " F_F1:" << func_pos_list[0];
      if (func_pos_list.size() > 1)    os << " F_F2:" << func_pos_list[1];
      if (func_pos_list.size() > 2)    os << " F_F3:" << func_pos_list[2];
      if (func_pos_list.size() > 3)    os << " F_F4:" << func_pos_list[3];
      if (morph[i][func_id].size() > 4 && morph[i][func_id][4] != "*" && morph[i][func_id][4] != "") 
	os << " F_F5:" << morph[i][func_id][4];
      if (morph[i][func_id].size() > 5 && morph[i][func_id][5] != "*" && morph[i][func_id][5] != "") 
	os << " F_F6:" << morph[i][func_id][5];

      if (-1 != re_case.search(morph[i][func_id][3])) os << " G_CASE:" << morph[i][func_id][0];

      os << " B:" << morph[i][head_id][3];

      if (-1 != re_dyn_a.search(morph[i][func_id][3])) 
	os << " A:" << morph[i][func_id][0];
      else if (morph[i][func_id].size() > 5 && morph[i][func_id][5] != "*" && morph[i][func_id][5] != "")
	os << " A:" << morph[i][func_id][5];
      else 
	os << " A:" << morph[i][func_id][3];
    }

    // chunk
    string chunk = "";
    for (unsigned int j = 0; j < morph[i].size(); j++) chunk += morph[i][j][0];

    if (-1 != re_kutouten.search(chunk)) {
      os << " G_PUNC:" << re_kutouten.getItem(1);
      os << " F_PUNC:" << re_kutouten.getItem(1);
    }
      
    if (-1 != re_open_bracket.search(chunk)) {
      os << " G_OB:" << re_open_bracket.getItem(1);
      os << " F_OB:" << re_open_bracket.getItem(1);
    }
      
    if (-1 != re_close_bracket.search(chunk)) {
      os << " G_CB:" << re_close_bracket.getItem(1);
      os << " F_CB:" << re_close_bracket.getItem(1);
    }

    if (i == 0) os << " F_BOS:1";
    if (i == (morph.size() - 1)) os << " F_EOS:1";

    os << endl;
    copy (morph2[i].begin(), morph2[i].end(), ostream_iterator<string>(os, "\n"));
  }

  os << "EOS" << endl << flush;
  return os;
}

int Selector::parse()
{
  return 1;
}
};
