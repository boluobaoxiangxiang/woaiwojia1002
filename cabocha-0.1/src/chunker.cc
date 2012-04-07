/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: chunker.cc,v 1.12 2001/07/01 18:01:45 taku-ku Exp $;

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

#include "chunker.h"
#include "common.h"

namespace CaboCha {

Chunker::Chunker(Param &q)
{
  try {
    if (q.chunkerModelFileName == "") CABOCHA_EXIT;
    YamCha::Param p;
    char *tmp[] = {0, "-m", 0, 0};
    tmp[2] = const_cast<char *>(q.chunkerModelFileName.c_str());
    p.set(3, tmp);
    chunker.set(p);
  }

  catch (string &e) {
    cerr << e << endl;
    CABOCHA_EXIT;
  }
}

Chunker::~Chunker() {}

istream &Chunker::read(istream &is)
{
  clear();
  chunker.clear();

  while (1) {
    string line;
 
    if (! getline(is,line)) {
//      is.setstate(ios::eofbit);
      is.clear(ios::eofbit|ios::badbit);
      return is;
    }
     
    if (line == "EOS" || line == "") return is;
  
    vector <string> tmp;
    vector <string> out;
    int _size = YamCha::split_string(line,"\t ",tmp);

    if (_size == 11) {  // JUMAN
      out.push_back(tmp[0]);  out.push_back(tmp[3]);
      out.push_back(tmp[5]);  out.push_back(tmp[7]);
      out.push_back(tmp[9]);  out.push_back("");  out.push_back("");

      line =  tmp[0];
      line += " "; line += tmp[1]; line += " "; line += tmp[2]; line += " "; line += tmp[3]; 
      line += " "; line += tmp[5]; line += " "; line += tmp[7]; line += " "; line += tmp[9];

    } else if (_size >= 4) { // ChaSen
      if (_size == 4) tmp.push_back("*"); // RWCP style
      if (tmp[4] == "") tmp[4] = "*";
      for (int i = tmp.size(); i < 6; i++) tmp.push_back("*");
      vector <string> tmp2;
      YamCha::split_string(tmp[3],"-",tmp2);
      for (int i = tmp2.size(); i < 4; i++) tmp2.push_back("*");
      out.push_back(tmp[0]);
      out.push_back(tmp2[0]); out.push_back(tmp2[1]); out.push_back(tmp2[2]); out.push_back(tmp2[3]);
      out.push_back(tmp[4]);  out.push_back(tmp[5]);
    } else {
      cerr << "FATAL: Format error " << line << endl;
      exit(-1);
    }

    lineBuf.push_back(line);
    chunker.add(out);
  }
}

ostream &Chunker::write(ostream &os)
{
  int k = 0;
  if (result.size()) result[0] = 1; // force to rewite! 
  for (unsigned int i = 0; i < lineBuf.size(); i++) {
    if (result[i]) os << "* " << k++ << " -1O" << endl; // Kyoto Univ. Corpus Format
    os << lineBuf[i] << endl;
  }
  os << "EOS" << endl;
  return os;
}

int Chunker::parse()
{
  chunker.parse();
  result.clear();
  for (unsigned int i = 0; i < lineBuf.size(); i++) {
    if (chunker.getTag(i) == "B") result.push_back(1);
    else result.push_back(0); // save the location of beggining the chunk
  }
  return 1;
}
}
