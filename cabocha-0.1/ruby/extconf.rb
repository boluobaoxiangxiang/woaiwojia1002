require 'mkmf'

dir_config('yamcha')

$LDFLAGS += ''
$CC = "c++"

if ENV["CFLAGS"]
   $CFLAGS  += ENV["CFLAGS"]
end

if ENV["LDFLAGS"]
  $LDFLAGS +=  ENV["LDFLAGS"]
end

$LDFLAGS += '-L/usr/lib -lyamcha -lcabocha -lstdc++'
create_makefile('CaboCha')
