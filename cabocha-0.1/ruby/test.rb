#!/usr/bin/ruby

require 'CaboCha'
# c = CaboCha.new
c = CaboCha.new("-f 1")
c2 = CaboCha.new("-f 0")
c3 = CaboCha.new("-I0 -O2")

puts  c.parse("��Ϻ�ϼ�Ϻ�����äƤ����ܤ�ֻҤ��Ϥ�����");
puts  c2.parse("��Ϻ�ϼ�Ϻ�����äƤ����ܤ�ֻҤ��Ϥ�����");
puts  c3.parse("��Ϻ�ϼ�Ϻ�����äƤ����ܤ�ֻҤ��Ϥ�����");


