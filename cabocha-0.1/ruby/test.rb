#!/usr/bin/ruby

require 'CaboCha'
# c = CaboCha.new
c = CaboCha.new("-f 1")
c2 = CaboCha.new("-f 0")
c3 = CaboCha.new("-I0 -O2")

puts  c.parse("太郎は次郎が持っている本を花子に渡した。");
puts  c2.parse("太郎は次郎が持っている本を花子に渡した。");
puts  c3.parse("太郎は次郎が持っている本を花子に渡した。");


