# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

# $Id: test.pl,v 1.3 2001/06/18 16:05:24 taku-ku Exp $;

use lib "../src/.libs";
use lib $ENV{PWD} . "/blib/lib";
use lib $ENV{PWD} . "/blib/arch";

BEGIN { $| = 1; print "1..1\n"; }
END {print "not ok 1\n" unless $loaded;}
use Text::CaboCha;
$loaded = 1;
print "ok 1\n";

$c  = new Text::CaboCha("");
$c2 = new Text::CaboCha("-f 1");
$c3 = new Text::CaboCha("-O 2");

print  $c->parse("太郎は次郎が持っている本を花子に渡した。");
print $c2->parse("太郎は次郎が持っている本を花子に渡した。");
print $c3->parse("太郎は次郎が持っている本を花子に渡した。");

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

