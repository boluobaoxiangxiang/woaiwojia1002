/*
 CaboCha -- Yet Another Japanese Dependency Structure Analyzer

 $Id: DynamicLibrary.h,v 1.1 2001/06/19 09:30:23 taku-ku Exp $;

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

#ifndef _DYNAMIC_LIBRARY_H
#define _DYNAMIC_LIBRARY_H
#include "common.h"

typedef int (*DLPROC)(...);

class DynamicLibrary
{
 private:
#ifdef _WIN32
  HINSTANCE libHandle;
#else
  void *libHandle;
#endif

 public:
  DynamicLibrary  (): libHandle(0) {};
  DynamicLibrary  (const char *libName) { load (libName); };
  ~DynamicLibrary () { unload (); };

  int load (const char *libName) 
  {
    if (libName) {
#ifdef _WIN32
      libHandle = LoadLibrary (libName);
#else
      libHandle = dlopen (libName, RTLD_LAZY);
#endif
    }

    return (libHandle != 0);
  }

  int unload ()
  {
    if (libHandle) {
#ifdef _WIN32
      FreeLibrary (libHandle);
#else
      dlclose (libHandle);
#endif
    }

    libHandle = 0;
    return 1;
  }

  int isloaded() const { return (libHandle != 0); };

  DLPROC getProcAddr (const char *procName) const
  {
    DLPROC proc = 0;

    if (libHandle) {
#ifdef _WIN32
      proc = (DLPROC) GetProcAddress (libHandle, procName);
#else
      proc = (DLPROC) dlsym (libHandle, procName);
#endif
    }

    return proc;
  }
};

#endif
