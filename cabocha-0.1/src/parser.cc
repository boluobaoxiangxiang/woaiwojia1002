/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: parser.cc,v 1.17 2001/07/07 21:02:40 taku-ku Exp $;

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

#include "parser.h"
#include "common.h"

namespace CaboCha {

Parser::Parser(Param &q)
{
  try {
    if (q.parserModelFileName == "") CABOCHA_EXIT

    switch (q.outputFormat) {
    case Param::LATTICE:
      _write = &Parser::_write_lattice;
      break;
    case Param::TREE:
      _write = &Parser::_write_tree;
      break;
    case Param::TREE_LATTICE:
      _write = &Parser::_write_tree_lattice;
      break;
    default:
      throw string ("Unkown output format");
    }

    isWriteHeader = 0;
    partialParsing = q.withCandidates;
    actionMode = q.actionMode;
    if (actionMode == Param::PARSER_MODE) {
      model.readModel(q.parserModelFileName);
    } else if (actionMode == Param::SELECTION_MODE) {
      _write = &Parser::_write_selection_mode;
      partialParsing = 1;
    } else {
      throw string ("Unkown action mode");
    }
  } 

  catch (string &e) {
    cerr << "Parser::Parser(): " << e << endl;
    CABOCHA_EXIT
  }
}

Parser::~Parser() {}

istream &Parser::read(istream &is)
{
  try {
    clear();
    string chunk_str = "";
    vector < vector <string> > gap_list;

    while (1) {
      string line;
      if (! getline(is,line)) {
//	is.setstate(ios::eofbit);
        is.clear(ios::eofbit|ios::badbit);
	return is;
      }

      if (line == "EOS" || line == "") {
	if (chunk_str != "") chunk.push_back(chunk_str);

	int _size = static_feature.size() - 1;
	gap.resize(_size*(_size+3)/2 + 1);
	for (unsigned int k = 0; k < gap_list.size(); k++) 
	  for (unsigned int i = 0; i < k; i++) 
	    for (unsigned int j = k + 1; j < gap_list.size(); j++) 
	      copy(gap_list[k].begin(), gap_list[k].end(), back_inserter(gap[j*(j+1)/2+i]));

	return is;
      }

      vector <string> tmp;
      YamCha::split_string(line,"\t ",tmp); 

      if (tmp[0] == "*" && tmp.size() >= 5) {
	vector <string> tmpv; // push dummy field
	static_feature.push_back(tmpv);
	dyn_a_feature.push_back(tmpv);
	dyn_b_feature.push_back(tmpv);
	dyn_a.push_back(tmpv);
	dyn_b.push_back(tmpv);
	gap_list.push_back(tmpv);
	morph.push_back(tmpv);

	unsigned int _size = static_feature.size() - 1;
	for (unsigned int i = 4; i < tmp.size(); i++) {
	  if (tmp[i] == "") continue;
	  switch (tmp[i][0]) {
	  case 'F':
	    static_feature[_size].push_back(tmp[i]);
	    break;
	  case 'G':
	    gap_list[_size].push_back(tmp[i]);
	    break;
	  case 'A':
	    dyn_a_feature[_size].push_back(tmp[i]);
	    break;
	  case 'B':
	    dyn_b_feature[_size].push_back(tmp[i]);
	    break;
	  default:
	    string e = "Unkown feature type [" + tmp[i] + "]";
	    throw e;
	  }
	}

	// other info
	comment.push_back (tmp[3]);
	score.push_back (0.0);

	// answer dep
	if (partialParsing) {
	   int p = atoi (tmp[2].c_str());
	   if (p >= 0 && p <= (int)answer.size()) 
	     throw string ("Given answer is invalid, " + tmp[2]) ;
	   answer.push_back(p);
	 
	   // answer type
	   unsigned int i;
	   for (i = 0; i < tmp[2].size(); i++) 
	     if (! ((tmp[2][i] >= '0' && tmp[2][i] <= '9') || 
		    (i == 0 && (tmp[2][i] == '+' || tmp[2][i] == '-')))) break;
	   answer_type.push_back(string(tmp[2].c_str() + i));
	} else {
	   answer.push_back(-1);
	   answer_type.push_back("O");
	}

	// init result
	result.push_back (-1);
	result_type.push_back ("O");

	if (chunk_str != "") chunk.push_back(chunk_str);
	chunk_str = "";

      } else {

	chunk_str += tmp[0];
	if (morph.size() == 0) {
	  string e =  "format error [" + line +  "]";
	  throw e;
	}

	morph[morph.size()-1].push_back(line);
      }
    }

    return is;
  }

  catch (string &e) {
    cerr << "Parser::read() " << e << endl;
    exit (EXIT_FAILURE);
  }
}

ostream &Parser::_write_selection_mode(ostream &os)
{
  if (! isWriteHeader) {
     os << "Version: " << VERSION << endl;
     os << "Package: " << PACKAGE << endl;
     os << endl;
     isWriteHeader = 1;
  }
   
  for (unsigned int i = 0; i < feature_list.size(); i++) {
     copy (feature_list[i].begin(), feature_list[i].end(), ostream_iterator<string>(os, " "));
     os << endl;
  }

  feature_list.clear();
  return os;
}

ostream &Parser::_write_lattice(ostream &os)
{
  os.setf(ios::fixed, ios::floatfield);
  os.precision(8);
  for (unsigned int i = 0; i < static_feature.size(); i++) {
    os << "* " << i << " " << result[i] << result_type[i] << " " << comment[i] << " " << score[i] << endl;
    copy (morph[i].begin(), morph[i].end(), ostream_iterator<string>(os, "\n"));
  }
  os << "EOS" << endl;

  return os;
}
   
ostream &Parser::_write_tree_lattice(ostream &os)
{
   _write_tree(os);
   return _write_lattice(os);
}

ostream &Parser::_write_tree(ostream &os)
{
  int _size = (int)(static_feature.size());
  vector <int> e(_size); fill(e.begin(), e.end(), 0);
  
  for (int i = 0; i < _size; i++) {
    int isdep = 0;
    for (int k = 0; k < i; k++) os << "  "; 
    os.width(42); os.setf(ios::right,ios::adjustfield);
    os << chunk[i].c_str();
    for (int j = i+1; j < _size; j++) {
      if (result[i] == j) {
	os << "-" << result_type[i];
	isdep = 1;
	e[j] = 1;
      } else if (e[j]) {
	os <<  " |";
      } else if (isdep) {
	os << "  ";
      } else {
	os << "--";
      }
    }
    os << endl;
  }

  os << "EOS" << endl;
  return os;
}

int Parser::estimate (int src, int dst, string &type, double &score)
{
  if (actionMode == Param::PARSER_MODE && answer[src] != -1) {
     if (answer[src] == dst) {
       type = answer_type [src];
       return 1;
     } else {
       type = "O";
       return 0;
     }
  }
   
  int dist = dst - src;
  vector <string> feature;

  // static feature
  for (unsigned int i = 0; i < static_feature[src].size(); i++) feature.push_back("S_" + static_feature[src][i]);
  for (unsigned int i = 0; i < static_feature[dst].size(); i++) feature.push_back("D_" + static_feature[dst][i]);

  if (dist == 1)                   feature.push_back("DIST:1");
  else if (dist >= 2 && dist <= 5) feature.push_back("DIST:2-5");
  else                             feature.push_back("DIST:6-");

  // gap feature
  int k = dst * (dst + 1) / 2 + src;
  for (unsigned int i = 0; i < gap[k].size(); i++) feature.push_back(gap[k][i]);

  // dynamic features
  for (unsigned int i = 0; i < dyn_a[dst].size(); i++) feature.push_back("D_" + dyn_a[dst][i]);
  for (unsigned int i = 0; i < dyn_a[src].size(); i++) feature.push_back("S_" + dyn_a[src][i]);
  for (unsigned int i = 0; i < dyn_b[dst].size(); i++) feature.push_back("D_" + dyn_b[dst][i]);

  // sort + uniq
  sort (feature.begin(),feature.end());
  vector <string>::iterator new_end = unique(feature.begin(), feature.end());
  feature.erase(new_end, feature.end());

  if (actionMode == Param::PARSER_MODE) { // test mode
    unsigned int max_score = 0; 
    unsigned int id        = 0;
    char **tmp = new char * [feature.size()];
    for (unsigned int i = 0; i < feature.size(); i++) tmp[i] = const_cast<char *>(feature[i].c_str());

    YamCha::ModelResult *result = model.classify(tmp, feature.size());
    for (unsigned int i = 0; i < model.classSize; i++) {
      if (max_score < result[i].voteScore) { // get max item only
        max_score = result[i].voteScore; 
        id = i; // save current id
      }
    }

    delete [] tmp;
    score = result[id].distScore;
    type  = result[id].className;
    return (type != "O");

  } else {

    score = 1.0;
    type = "O";
    int isdep = 0;

    if (answer[src] == dst) {
      isdep = 1;
      type = answer_type[src];
    }
     
    vector <string> tmp;
    tmp.push_back(type);
    copy (feature.begin(), feature.end(), back_inserter(tmp));
    feature_list.push_back(tmp);
    return isdep;
  }

  return 0;
}

int Parser::parse() 
{
  int _size = static_feature.size();
  if (_size == 0) return 0;

  vector <int> del  (_size);  fill(del.begin(),  del.end(), 0);
  vector <int> head (_size);  fill(head.begin(), head.end(), -1);
  vector <int> list (_size);
  string type;

  if (actionMode == Param::PARSER_MODE) {
    for (int i = 0; i < (_size - 1); i++) 
      for (int j = i; j < answer[i]; j++) head[j] = answer[i];
  }

  while (1) {
    list.clear();
    for (unsigned int i = 0; i < del.size(); i++) if (! del[i]) list.push_back(i);
    if (list.size() <= 1) break; 

    int prev   = 0;
    int change = 0;

    for (int i = 0; i < (int)(list.size()-1); i++) {
      int    src   = list[i];
      int    dst   = list[i+1];
      int    isdep = 1;
      double sc    = 0.0;

      if (src != _size-2 && result[src] != dst) 
	 isdep = estimate(src, dst, type, sc);

      if (i == (int)(list.size()-2) && ! change && ! isdep ) isdep = 1;
      if (head[src] == dst) isdep = 1;

      if (isdep) {
	if (result[src] != dst) {
	  copy (dyn_b_feature[dst].begin(), dyn_b_feature[dst].end(), back_inserter (dyn_b[src]));
	  copy (dyn_a_feature[src].begin(), dyn_a_feature[src].end(), back_inserter (dyn_a[dst]));
	  score[src] = sc;
	  result_type[src] = type;
	}

	result[src] = dst;
	if (! del[src]) change = 1;
	if (! prev)     del[src] = 1;
      }

       prev = isdep;
    }
  }

  return 1;
}

}
