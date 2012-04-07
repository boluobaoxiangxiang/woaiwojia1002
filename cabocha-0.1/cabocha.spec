%define prefix /usr
%define version 0.1
%define release 1

Summary: Yet Another Japaese Depenedency Structuer Analyzer
Name: cabocha
Version: %{version}
Release: %{release}
Copyright: GPL2
Group: local
Packager: Taku Kudoh <taku-ku@is.aist-nara.ac.jp>
Source: %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}
Requires: yamcha >= 0.1 chasen >= 2.2.0

%description
Yet Another Japaese Depenedency Structuer Analyzer

%package devel
Summary: Libraries and header files for CaboCha
Group: Development/Libraries
Requires: TinySVM >= 0.02

%description devel
Libraries and header files for CaboCha

%package perl
Summary: CaboCha Perl Module
Group: Development/Libraries
Requires: perl >= 5.0 cabocha >= %{version}

%description perl
CaboCha Perl Module

%prep

%setup

%build
./configure --prefix=%{prefix}
make CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS"

cd perl
perl Makefile.PL
make INC="-I../src -I%{prefix}/include -I/usr/include" LCFLAGS="$RPM_OPT_FLAGS" LDDLFLAGS="-shared -Wl,-rpath -Wl,%{prefix}/lib -L../src/.libs -lcabocha -lchasen -lyamcha"
cd ..

%install
make prefix=$RPM_BUILD_ROOT%{prefix} modeldir=$RPM_BUILD_ROOT%{prefix}/lib/cabocha/model install

cd perl
make PREFIX=$RPM_BUILD_ROOT%{prefix} INSTALLMAN3DIR=$RPM_BUILD_ROOT%{prefix}/man/man3 install
cd ..

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)
%doc README AUTHORS ChangeLog COPYING doc/*.html doc/*.png doc/*.c doc/*.txt
%{prefix}/lib/cabocha/*
%{prefix}/lib/*.so.*
%{prefix}/bin/*
%{prefix}/man/*/cabocha.1*
%{prefix}/libexec/*
%config %{prefix}/etc/cabocharc

%files devel
%defattr(-, root, root)
%{prefix}/include/*
%{prefix}/lib/*.so
%{prefix}/lib/*.a
%{prefix}/lib/*.la

%files perl
%defattr(-, root, root)
%{prefix}/lib/perl5/site_perl/*
%{prefix}/man/man3/*
