#include <cabocha.h>
#include <stdio.h>

// $Id: example.c,v 1.2 2001/06/30 19:45:50 taku-ku Exp $;

// How to compile?
//  % gcc -O2 `cabocha-config -cflags` example.c -o example `cabocha-config -libs`
//
//  You may have to add '-lstdc++' to linker option if you use GCC.
//  % gcc -O2 `cabocha-config -cflags` example.c -o example `cabocha-config -libs` -lstdc++

int main (int argc, char **argv) 
{
   cabocha_t *c;
   char p[1024] = "太郎は次郎が持っている本を花子に渡した。";

   c = cabocha_getopt_argv(argc, argv);

   printf ("INPUT: %s\n", p);
   printf ("RESULT:\n%s", cabocha_sparse_tostr(c, p));

   cabocha_destroy(c);
   
   return 0;
}
