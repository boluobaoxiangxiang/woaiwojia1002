# Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.12 2001/07/07 21:02:40 taku-ku Exp $;
#
AUTOMAKE_OPTIONS = no-dependencies
INCLUDES = @YAMCHA_CFLAGS@ @CHASEN_CFLAGS@ -DCABOCHA_DEFAULT_RC="\"$(CABOCHA_DEFAULT_RC)\""
lib_LTLIBRARIES = libcabocha.la
libcabocha_la_LDFLAGS = -no-undefined -version-info $(LTVERSION)
libcabocha_la_SOURCES = getopt.c getopt.h getopt1.c \
                 regex.c regex.h common.h \
		 parser.h parser.cc \
		 selector.h selector.cc \
		 chunker.h chunker.cc \
		 param.cc libcabocha.cc \
		 re.h re.cc chasenio.cc \
		 chasenio.h cabocha.h DynamicLibrary.h

include_HEADERS = cabocha.h

bin_PROGRAMS = cabocha
cabocha_SOURCES = cabocha.cc
cabocha_LDADD = libcabocha.la @YAMCHA_LIBS@

dll:
	$(CC) -shared -Wl,--output-def,libcabocha.def -o libcabocha.dll *.o -L. -lyamcha-win32 -lstdc++
 
