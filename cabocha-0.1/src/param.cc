/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: param.cc,v 1.22 2001/07/08 14:06:43 taku-ku Exp $;

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

#include "cabocha.h"
#include "common.h"
#include "re.h"

// $Id: param.cc,v 1.22 2001/07/08 14:06:43 taku-ku Exp $;
#define CABOCHA_DEFAULT_ACTION_MODE   Param::PARSER_MODE
#define CABOCHA_DEFAULT_OUTPUT_FORMAT Param::TREE

#ifdef HAVE_LIBCHASEN
#define CABOCHA_DEFAULT_INPUT_LAYER   Param::INPUT_RAW_SENTENCE
#else
#define CABOCHA_DEFAULT_INPUT_LAYER   Param::INPUT_POS
#endif
#define CABOCHA_DEFAULT_OUTPUT_LAYER  Param::OUTPUT_PARSE

extern char *optarg;
extern int optind;
static const char *short_options = "t:f:CI:O:m:o:c:M:r:vh";

static struct option long_options[] = {
  {"action-mode",     no_argument,       NULL, 't'},
  {"output-format",   required_argument, NULL, 'f'},
  {"candidate",       no_argument,       NULL, 'C'},
  {"input-layer",     required_argument, NULL, 'I'},
  {"output-layer",    required_argument, NULL, 'O'},
  {"parser-model",    no_argument,       NULL, 'm'},
  {"chunker-model",   required_argument, NULL, 'M'},
  {"rcfile",          required_argument, NULL, 'r'},
  {"chasenrc",        required_argument, NULL, 'c'},
  {"output-file",     required_argument, NULL, 'o'},
  {"version",         no_argument,       NULL, 'v'},
  {"help",            no_argument,       NULL, 'h'},
  {NULL, 0, NULL, 0}
};

namespace CaboCha {

Param::Param()
{
  clear();
}

void Param::clear()
{
  actionMode           = CABOCHA_DEFAULT_ACTION_MODE;
  outputFormat         = CABOCHA_DEFAULT_OUTPUT_FORMAT;
  inputLayer           = CABOCHA_DEFAULT_INPUT_LAYER;
  outputLayer          = CABOCHA_DEFAULT_OUTPUT_LAYER;
  parserModelFileName  = "";
  chunkerModelFileName = "";
  chasenrc             = "";
  outputFileName       = "";
  rcFileName           = "";
  withCandidates       = 0;
  inputFileNameList.clear();
}

int Param::set
(int argc, char **argv)
{
  optind = 1;
  int optnum = 0;

  try {
    while (1) {
      int opt = getopt_long (argc, argv, short_options, long_options, NULL);
      if (opt == EOF) break;
      optnum++;
      switch(opt) {
      case 't':
	actionMode   = atoi(optarg);
	break;
      case 'f':
	outputFormat = atoi(optarg);
	break;
      case 'C':
	withCandidates = 1;
	break;
      case 'I':
	inputLayer  = atoi(optarg);
	break;
      case 'O':
	outputLayer  = atoi(optarg);
	break;
      case 'm':
	parserModelFileName = string(optarg);
	break;
      case 'M':
	chunkerModelFileName = string(optarg);
	break;
      case 'r':
	rcFileName = string(optarg);
	break;
      case 'c':
	chasenrc = string(optarg);
	break;
      case 'o':
	outputFileName = string(optarg);
	break;
      case 'h':
	printHelp(cout);
	exit(EXIT_SUCCESS);
	break;
      case 'v':
	cout << PACKAGE << " of " << VERSION << endl;
	exit(EXIT_SUCCESS);
      default:
	return 0;
      }
    }
    
    if (rcFileName == "") rcFileName = getDefaultRc();
    loadRc(rcFileName);

    for (int i = optind; i < argc; i++) inputFileNameList.push_back(string(argv[i]));
    return optnum;
  }

  catch (string &e) {
    cerr << e << endl;
    exit(EXIT_FAILURE);
  }
}

int Param::set(const string &s)
{
  vector <string> tmp;
  if (! YamCha::split_string(s,"\t ", tmp)) return 0;
  int argc = tmp.size() + 1;
  char **argv = new char * [argc];

  argv[0] = "Param::set";
  int j = 1;
  for (unsigned int i = 0; i < tmp.size(); i++) {
    if (tmp[i] == "") continue;
    argv[j++] = const_cast<char *>(tmp[i].c_str());
  }
  argc = j;
  int r = set(argc, argv);
  delete [] argv;
  return r;
}

void Param::printHelp(ostream &os)
{
  os << COPYRIGHT << endl;
  os << "Usage: cabocha [optsions] <files>..." << endl;
  os << endl;
  os << "  -t, --action-mode=TYPE    set action mode" << endl;
  os << "                              0 - parser mode (default)" << endl;
  os << "                              1 - training mode" << endl;
  os << "  -f, --output-format=TYPE  set output format type" << endl;
  os << "                              0 - tree (default)" << endl;
  os << "                              1 - lattice" << endl;
  os << "                              2 - tree+lattice" << endl;
  os << "  -I, --input-layer=LAYER   set input layer" << endl;
  os << "                              0 - raw sentence layer (tagged with ChaSen, default)" << endl;
  os << "                              1 - POS tagged layer " << endl;
  os << "                              2 - POS tagged and Chunked layer" << endl;
  os << "                              3 - POS tagged, Chunked and Feature selected layer" << endl;
  os << "  -O, --output-layer=LAYER  set ouput layer" << endl;
  os << "                              1 - POS tagged layer (tagged with ChaSen)" << endl;
  os << "                              2 - Chunked layer" << endl;
  os << "                              3 - Chunked and Feature selected layer" << endl;
  os << "                              4 - Parsed layer (default)" << endl;
  os << "  -m, --parser-model=FILE   use FILE as parser model file" << endl;
  os << "  -M, --chunker-model=FILE  use FILE as chunker model file" << endl;
  os << "  -C, --candidate           partial parsing mode" << endl;
  os << "  -r, --rcfile=FILE         use FILE as rc file" << endl;
  os << "                             (default: " << CABOCHA_DEFAULT_RC << ")" << endl;
  os << "  -c, --chasenrc=FILE       use FILE as chasenrc" << endl;
  os << "  -o, --output=FILE         set the output file name" << endl;
  os << "  -v, --version             show the version of CaboCha and exit." << endl;
  os << "  -h, --help                show this help and exit." << endl;
}

string Param::getDefaultRc()
{
#ifdef HAVE_GETENV
  char *homedir = getenv("HOME");
  if (homedir) {
    string s(homedir);
#ifdef _WIN32
    s += "\\.cabocharc";
#else
    s += "/.cabocharc";
#endif
    ifstream ifs(s.c_str());
    if (ifs) return s;
  }

  char *rcenv = getenv ("CABOCHARC");
  if (rcenv) string(rcenv);
#endif

#if defined HAVE_WINDOWS_H && defined _WIN32
  char filename[1024];
  GetPrivateProfileString("cabocha", "cabocharc", CABOCHA_DEFAULT_RC,
  			  filename, sizeof(filename), "cabocha.ini");
  return string(filename);
#endif

  return string(CABOCHA_DEFAULT_RC);
}

int Param::loadRc(const string &filename)
{
  ifstream ifs(filename.c_str());
  if (!ifs) {
    cerr << filename << " : no such file or directory, Ignored." << endl;
    return 0;
  }

  Regex re_contents;
  re_contents.compile("^([a-zA-Z\\-]+)\\s*=\\s*([^=]+)\\s*$");
  string line;
  while (getline(ifs, line)) {
    if ((line != "" && line[0] == '#') || line == "") continue;
    if (re_contents == line) { // match
      string item = re_contents.getItem(1);
      string value = re_contents.getItem(2);
      if (item == "action-mode"        && actionMode == CABOCHA_DEFAULT_OUTPUT_FORMAT) 
	actionMode = atoi(value.c_str());
      else if (item == "output-format" && outputFormat == CABOCHA_DEFAULT_OUTPUT_LAYER) 
	outputFormat = atoi(value.c_str());
      else if (item == "input-layer"   && inputLayer == CABOCHA_DEFAULT_INPUT_LAYER) 
	inputLayer = atoi(value.c_str());
      else if (item == "output-layer"  && outputLayer == CABOCHA_DEFAULT_OUTPUT_LAYER) 
	outputLayer = atoi(value.c_str());
      else if (item == "parser-model"  && parserModelFileName == "") 
	parserModelFileName = value;
      else if (item == "chunker-model" && chunkerModelFileName == "") 
	chunkerModelFileName = value;
      else if (item == "chasenrc" && chasenrc == "") 
	chasenrc = value;
    }
  } 

  return 1;
}
}
