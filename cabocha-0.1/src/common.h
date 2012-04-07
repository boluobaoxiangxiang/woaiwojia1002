/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: common.h,v 1.13 2001/06/19 09:30:23 taku-ku Exp $;

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

#ifndef _CABOCHA_COMMON_H
#define _CABOCHA_COMMON_H

// $Id: common.h,v 1.13 2001/06/19 09:30:23 taku-ku Exp $;
#define COPYRIGHT "CaboCha (Yet Another Japanese Dependency Structure Analyzer)\n\
\nCopyright (C) 2001 Taku Kudoh All rights reserved.\n"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include "regex.h"

#ifndef _WIN32
#define DEFAULT_MBCTYPE MBCTYPE_EUC
#else
#define DEFAULT_MBCTYPE MBCTYPE_SJIS
#endif
   
#ifdef STDC_HEADERS
#include <stdio.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
   
#ifdef  HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#if defined HAVE_GETOPT_H && defined HAVE_GETOPT_LONG
#include <getopt.h>
#else
#include "getopt.h"
#endif

#ifdef __cplusplus
}
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#define CABOCHA_EXIT { cout << COPYRIGHT << "`" << PACKAGE << " --help' for more information." << endl; \
                       exit(EXIT_FAILURE); }
#endif

