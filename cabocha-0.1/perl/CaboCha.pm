package Text::CaboCha;

require 5.005;
require Exporter;
require DynaLoader;

@ISA = qw(Exporter DynaLoader);
%EXPORT_TAGS = ( 'all' => [ qw() ] );
@EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );
@EXPORT = qw();
$VERSION = '0.01';

bootstrap Text::CaboCha $VERSION;
1;
__END__
# Below is stub documentation for your module. You better edit it!

=head1 NAME

Text::CaboCha - Perl extension for CaboCha

=head1 SYNOPSIS

  use Text::CaboCha;

=head1 DESCRIPTION

use Text::CaboCha;

$c  = new Text::CaboCha("");
print  $c->parse("太郎は次郎が持っている本を花子に渡した。");

$c2  = new Text::CaboCha("-f1");
print $c2->parse("太郎は次郎が持っている本を花子に渡した。");

$c3  = new Text::CaboCha("-f1 -O2");
print  $c3->parse("太郎は次郎が持っている本を花子に渡した。");

=head2 EXPORT

=head1 AUTHOR

Taku Kudoh <taku-ku@is.aist-nara.ac.jp>

=head1 SEE ALSO

perl(1). cabocha(1).

=cut
