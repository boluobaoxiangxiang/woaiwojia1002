# Generated automatically from Makefile.PL.in by configure.
use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    'CC'                => 'c++',
    'LD'                => 'c++',
    'NAME'		=> 'Text::CaboCha',
    'VERSION_FROM'	=> 'CaboCha.pm', # finds $VERSION
    'INC'               => '-I/usr/local/include',
    'LIBS'              => '-L/usr/local/lib -L/usr/lib -lyamcha -lcabocha'
);
