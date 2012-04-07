# Process this file with automake to produce Makefile.in
#
# $Id: Makefile.am,v 1.23 2001/07/08 14:06:43 taku-ku Exp $;
#
@SET_MAKE@
AUTOMAKE_OPTIONS = 1.4 no-dependencies
SUBDIRS = src man model tests doc libexec training
confdir = $(sysconfdir)
conf_DATA = cabocharc
EXTRA_DIRS = perl ruby
EXTRA_DIST = cabocha.spec training/Makefile.in
bin_SCRIPTS = cabocha-config

dist-hook:
	for subdir in $(EXTRA_DIRS); do \
	  cp -rp $$subdir $(distdir)/$$subdir; \
	  rm -f $(distdir)/$$subdir/*~; \
	  rm -f $(distdir)/$$subdir/*.{bak,orig}; \
	  rm -rf $(distdir)/$$subdir/CVS; \
	  rm -rf $(distdir)/$$subdir/*/CVS; \
	done

rpm: dist
	rpm -ta @PACKAGE@-@VERSION@.tar.gz

win:
	make distclean
	nkf -s src/selector.h >  src/selector-sjis.h
	env OBJDUMP=i386-mingw32msvc-objdump AS=i386-mingw32msvc-as \
	AR=i386-mingw32msvc-ar RANLIB=i386-mingw32msvc-ranlib \
	CXX=i386-mingw32msvc-g++ CC=i386-mingw32msvc-gcc \
	NM=i386-mingw32msvc-nm DLLTOOL=i386-mingw32msvc-dlltool \
	./configure --disable-shared --program-suffix=.exe \
	--host=i386-pc-mingw32 --target=i386-pc-mingw32
	make CFLAGS+=-DCABOCHA_DLL_EXPORT LIBS+="-L. -lyamcha-win32 -lstdc++"
	rm -f src/selector-sjis.h
	(cd src; make dll)

win-dist:
	rm -f @PACKAGE@-@VERSION@.zip
	make prefix=`pwd`/@PACKAGE@-@VERSION@ modeldir=`pwd`/@PACKAGE@-@VERSION@/model install
	rm -fr ./@PACKAGE@-@VERSION@/lib
	rm -fr ./@PACKAGE@-@VERSION@/doc/CVS
	rm -fr ./@PACKAGE@-@VERSION@/doc/Makefile*
	rm -fr ./@PACKAGE@-@VERSION@/man
	rm -fr ./@PACKAGE@-@VERSION@/libexec
	rm -fr ./@PACKAGE@-@VERSION@/bin/cabocha-config.exe
	mkdir -p ./@PACKAGE@-@VERSION@/lib
	cp -fr doc ./@PACKAGE@-@VERSION@	
	cp -fr src/libcabocha.dll ./@PACKAGE@-@VERSION@/lib
	cp -fr src/libcabocha.def ./@PACKAGE@-@VERSION@/lib
	perl -pe 's#\@DEFAULT_INPUT_LAYER\@#0#; \
		s#\@prefix\@/lib/cabocha/model/#c:\\cabocha\\model\\#g; \
		s#\.model#.sjis-model#;' < cabocharc.in \
		| nkf -sc > ./@PACKAGE@-@VERSION@/etc/cabocharc
	cp -f cabocha.ini ./@PACKAGE@-@VERSION@
	cp -f README COPYING AUTHORS ChangeLog @PACKAGE@-@VERSION@
	zip -r @PACKAGE@-@VERSION@.zip @PACKAGE@-@VERSION@
	rm -fr @PACKAGE@-@VERSION@

dist-all-package:
	(test -f Makefile) && make distclean
	./configure
	make dist
	make rpm
	make clean
	make win
	make win-dist
