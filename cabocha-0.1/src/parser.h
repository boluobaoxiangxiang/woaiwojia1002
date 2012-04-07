/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: parser.h,v 1.8 2001/06/24 02:09:56 taku-ku Exp $;

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

#ifndef _CABOCHA_PARSER_H
#define _CABOCHA_PARSER_H

#include "cabocha.h"

namespace CaboCha
{
class Parser: public Base
{
private:
  YamCha::svmModel model;
  vector < vector <string> > static_feature;
  vector < vector <string> > dyn_b_feature;
  vector < vector <string> > dyn_b;
  vector < vector <string> > dyn_a_feature;
  vector < vector <string> > dyn_a;
  vector < vector <string> > gap;
  vector < vector <string> > feature_list;
  int isWriteHeader;
  int actionMode;
  int partialParsing;
  int estimate(int, int, string&, double&);
  ostream &_write_tree(ostream &);
  ostream &_write_lattice(ostream &);
  ostream &_write_tree_lattice(ostream &);   
  ostream &_write_selection_mode(ostream &);
  ostream &(Parser::*_write)(ostream &);

public:
  vector <int>    result;
  vector <string> result_type;
  vector <int>    answer;
  vector <string> answer_type;
  vector <string> chunk;
  vector <string> comment;
  vector <double> score;
  vector < vector <string> > morph;

  Parser(Param &);
  ~Parser();
  int parse();
  istream &read(istream &);
  ostream &write(ostream &os) { return (this->*_write)(os); }
  unsigned int size() {  return static_feature.size(); }

  int clear()
  {
    static_feature.clear();
    dyn_b_feature.clear();
    dyn_b.clear();
    dyn_a_feature.clear();
    dyn_a.clear();
    gap.clear();
    chunk.clear();
    result.clear();
    result_type.clear();
    answer.clear();
    answer_type.clear();
    comment.clear();
    score.clear();
    morph.clear();
    feature_list.clear();
    return 1;
  }
};

}
#endif
