#! /usr/bin/perl -CSD
use utf8;
use FindBin;
use lib $FindBin::Bin;
use engtok;

my $light=shift(@ARGV); # si light==1, no separaria 'll 're 've ...

my $t = engtok->new(); 


while (<STDIN>) {

    print join(" ", $t->tokenize_string($_,$light)), "\n"; 

}

