/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: cabocha.h,v 1.19 2001/07/01 18:01:45 taku-ku Exp $;

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

#ifndef _CABOCHA_CABOCHA_H
#define _CABOCHA_CABOCHA_H

#ifdef __cplusplus

 /* for C */
extern "C" {
#endif
typedef struct {
  int allocated;
  void *ptr;
} cabocha_t;

#ifdef _WIN32
#  ifdef CABOCHA_DLL_EXPORT
#    define CABOCHA_DLL_EXTERN    __declspec(dllexport)
#  else
#    ifdef  CABOCHA_DLL_IMPORT
#      define CABOCHA_DLL_EXTERN  __declspec(dllimport)
#    endif
#  endif
#endif

#ifndef CABOCHA_DLL_EXTERN
#  define CABOCHA_DLL_EXTERN extern
#endif 

CABOCHA_DLL_EXTERN cabocha_t *cabocha_getopt_argv   (int, char **);
CABOCHA_DLL_EXTERN cabocha_t *cabocha_getopt_argv2  (char *);
CABOCHA_DLL_EXTERN char      *cabocha_sparse_tostr  (cabocha_t *, char *);
CABOCHA_DLL_EXTERN void       cabocha_destroy       (cabocha_t *);

#ifdef __cplusplus
}
#endif

/* for C++ */
#ifdef __cplusplus
#include <yamcha.h>

namespace CaboCha {
class Param
{
public:
  enum { PARSER_MODE, SELECTION_MODE };
  enum { TREE, LATTICE, TREE_LATTICE, XML };
  enum { INPUT_RAW_SENTENCE, INPUT_POS, INPUT_CHUNK, INPUT_SELECTION, INPUT_PARSE };
  enum { OUTPUT_RAW_SENTENCE, OUTPUT_POS, OUTPUT_CHUNK, OUTPUT_SELECTION, OUTPUT_PARSE };

  int actionMode;
  int outputFormat;
  int inputLayer;
  int outputLayer;
  int withCandidates;
  string chasenrc;
  string parserModelFileName;
  string chunkerModelFileName;
  string rcFileName;

  vector <string> inputFileNameList;
  string outputFileName;

  Param();
  void clear();
  int set(int, char **);
  int set(const string &);
  string getDefaultRc();
  int loadRc(const string &);
  void printHelp(ostream &);
};

class Base
{
public:
  Base() {};
  Base(Param &) {};
  virtual ~Base() {};

  virtual int parse() = 0;
  virtual istream &read(istream &) = 0;
  virtual ostream &write(ostream &) = 0;
  virtual unsigned int size() = 0;
  virtual int clear() = 0;
  friend ostream& operator<<(ostream& os, Base &b) { return b.write(os); }
  friend istream& operator>>(istream& is, Base &b) { return b.read(is);  }
  friend Base& operator|(Base &b1, Base &b2)
  {
    strstream strs;
    b1.parse();
    strs << b1;
    strs >> b2;
    b2.parse();
    return b2;
  }
};

class CaboCha: public Base
{
private:
  vector <Base *> list;
  string outbuf;

public:
  CaboCha(Param &);
  ~CaboCha();
  int parse();
  istream &read(istream &);
  ostream &write(ostream &);

  unsigned int size()          
  { 
    return list.size(); 
  }

  int clear ()
  { 
    for (unsigned int i =  0; i < list.size(); i++) list[i]->clear();
    return 1; 
  }

  unsigned int append(Base *b)
  {
    list.push_back(b); 
    return size(); 
  }

  unsigned int prepend(Base *b) 
  { 
    list.insert(list.begin(), b); 
    return size();
  }

  unsigned int insert(Base *b, unsigned int pos) 
  { 
    list.insert(list.begin() + pos, b); return size(); 
  }

  // special method
  const char *sparse_tostr(const char *);
};
}
#endif
#endif
